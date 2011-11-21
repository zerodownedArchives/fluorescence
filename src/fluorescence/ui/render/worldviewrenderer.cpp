
#include "worldviewrenderer.hpp"

#include <ui/manager.hpp>
#include <ui/render/renderqueue.hpp>
#include <ui/texture.hpp>
#include <ui/fontengine.hpp>
#include <ui/components/worldview.hpp>

#include <misc/log.hpp>

#include <world/ingameobject.hpp>
#include <world/lightmanager.hpp>
#include <world/manager.hpp>

#include <data/manager.hpp>
#include <data/huesloader.hpp>

namespace fluo {
namespace ui {

WorldViewRenderer::WorldViewRenderer(boost::shared_ptr<RenderQueue> renderQueue, components::WorldView* worldView) : IngameObjectRenderer(IngameObjectRenderer::TYPE_WORLD),
        worldView_(worldView), renderQueue_(renderQueue) {

    CL_GraphicContext gc = fluo::ui::Manager::getSingleton()->getGraphicContext();

    shaderProgram_.reset(new CL_ProgramObject(CL_ProgramObject::load(gc, "shader/world_vertex.glsl", "shader/world_fragment.glsl")));
    shaderProgram_->bind_attribute_location(0, "gl_Vertex");
    shaderProgram_->bind_attribute_location(1, "TexCoord0");
    shaderProgram_->bind_attribute_location(2, "gl_Normal");
    shaderProgram_->bind_attribute_location(3, "HueInfo0");

    if (!shaderProgram_->link()) {
        LOG_EMERGENCY << "Error while linking program:\n" << shaderProgram_->get_info_log().c_str() << std::endl;
        throw CL_Exception("Unable to link program");
    }
}

WorldViewRenderer::~WorldViewRenderer() {
    renderQueue_->clear();
}

void WorldViewRenderer::checkTextureSize() {
    if (!texture_ || texture_->getWidth() != worldView_->getWidth() || texture_->getHeight() != worldView_->getHeight()) {
        texture_.reset(new ui::Texture(false));
        texture_->initPixelBuffer(worldView_->getWidth(), worldView_->getHeight());
        texture_->setReadComplete();

        CL_GraphicContext gc = ui::Manager::getGraphicContext();

        depthTexture_ = CL_Texture(gc, gc.get_size(), cl_depth_component);
    }
}

boost::shared_ptr<Texture> WorldViewRenderer::getTexture(CL_GraphicContext& gc) {
    if (true || renderQueue_->requireWorldRepaint() || !texture_ || requireInitialRepaint()) {
        checkTextureSize();
        CL_FrameBuffer origBuffer = gc.get_write_frame_buffer();

        CL_FrameBuffer fb(gc);
        fb.attach_color_buffer(0, *texture_->getTexture());
        fb.attach_depth_buffer(depthTexture_);

        gc.set_frame_buffer(fb);

        render(gc);

        gc.set_frame_buffer(origBuffer);
    }

    return texture_;
}


void WorldViewRenderer::render(CL_GraphicContext& gc) {
    renderQueue_->preRender();


    gc.clear(CL_Colorf(0.f, 0.f, 0.f, 1.f));
    gc.clear_depth(1.0);

    CL_BufferControl buffer_control;
    buffer_control.set_depth_compare_function(cl_comparefunc_lequal);
    buffer_control.enable_depth_write(true);
    buffer_control.enable_depth_test(true);
    buffer_control.enable_stencil_test(false);
    buffer_control.enable_color_write(true);
    gc.set_buffer_control(buffer_control);


    gc.set_program_object(*shaderProgram_, cl_program_matrix_modelview_projection);

    int clippingLeftPixelCoord = worldView_->getCenterPixelX() - worldView_->getWidth()/2;
    int clippingRightPixelCoord = worldView_->getCenterPixelX() + worldView_->getWidth()/2;
    int clippingTopPixelCoord = worldView_->getCenterPixelY() - worldView_->getHeight()/2;
    int clippingBottomPixelCoord = worldView_->getCenterPixelY() + worldView_->getHeight()/2;


    CL_Vec2f pixelOffsetVec(clippingLeftPixelCoord, clippingTopPixelCoord);
    shaderProgram_->set_uniform2f("PositionOffset", pixelOffsetVec);

    gc.set_texture(0, *(data::Manager::getSingleton()->getHuesLoader()->getHuesTexture()->getTexture()));
    shaderProgram_->set_uniform1i("HueTexture", 0);
    shaderProgram_->set_uniform1i("ObjectTexture", 1);

    boost::shared_ptr<world::LightManager> lightManager = world::Manager::getLightManager();
    shaderProgram_->set_uniform3f("AmbientLightIntensity", lightManager->getAmbientIntensity());
    shaderProgram_->set_uniform3f("GlobalLightIntensity", lightManager->getGlobalIntensity());
    shaderProgram_->set_uniform3f("GlobalLightDirection", lightManager->getGlobalDirection());

    RenderQueue::const_iterator igIter = renderQueue_->begin();
    RenderQueue::const_iterator igEnd = renderQueue_->end();

    bool renderingComplete = true;

    batchFill_ = 0;
    lastTexture_ = NULL;

    unsigned long long minRenderDepth = renderQueue_->getMinRenderDepth();
    unsigned long long maxRenderDepth = renderQueue_->getMaxRenderDepth();
    float renderDiff = maxRenderDepth - minRenderDepth;

    //LOG_DEBUG << std::hex << "min=" << minRenderPrio << " diff=" << renderDiff << std::endl;

    igEnd = renderQueue_->end();

    for (; igIter != igEnd; ++igIter) {
        world::IngameObject* curObj = igIter->get();

        // check if current object is in the area visible to the player
        if (!curObj->isInDrawArea(clippingLeftPixelCoord, clippingRightPixelCoord, clippingTopPixelCoord, clippingBottomPixelCoord)) {
            continue;
        }

        // check if texture is ready to be drawn
        ui::Texture* tex = curObj->getIngameTexture().get();

        // happens e.g. for the equipped backpack
        if (!tex) {
            continue;
        }

        if (!tex->isReadComplete()) {
            renderingComplete = false;
            continue;
        }

        // object is invisible
        if (!curObj->isVisible()) {
            continue;
        }

        batchAdd(gc, curObj, tex, minRenderDepth, renderDiff);
    }

    batchFlush(gc);

    gc.reset_textures();
    gc.reset_program_object();

    gc.clear_depth(1.0);

    buffer_control.enable_depth_write(false);
    buffer_control.enable_depth_test(false);
    buffer_control.enable_stencil_test(false);
    buffer_control.enable_color_write(true);
    gc.set_buffer_control(buffer_control);

    renderQueue_->postRender(renderingComplete);
}

boost::shared_ptr<RenderQueue> WorldViewRenderer::getRenderQueue() const {
    return renderQueue_;
}

void WorldViewRenderer::batchAdd(CL_GraphicContext& gc, world::IngameObject* curObj, ui::Texture* tex, unsigned long long& minRenderDepth, float renderDiff) {
    static CL_Rectf texCoordHelper(0.0f, 0.0f, 1.0f, 1.0f);

    static CL_Vec2f texCoords[6] = {
        CL_Vec2f(texCoordHelper.left, texCoordHelper.top),
        CL_Vec2f(texCoordHelper.right, texCoordHelper.top),
        CL_Vec2f(texCoordHelper.left, texCoordHelper.bottom),
        CL_Vec2f(texCoordHelper.right, texCoordHelper.top),
        CL_Vec2f(texCoordHelper.left, texCoordHelper.bottom),
        CL_Vec2f(texCoordHelper.right, texCoordHelper.bottom)
    };

    static CL_Vec2f texCoordsMirrored[6] = {
        CL_Vec2f(texCoordHelper.right, texCoordHelper.top),
        CL_Vec2f(texCoordHelper.left, texCoordHelper.top),
        CL_Vec2f(texCoordHelper.right, texCoordHelper.bottom),
        CL_Vec2f(texCoordHelper.left, texCoordHelper.top),
        CL_Vec2f(texCoordHelper.right, texCoordHelper.bottom),
        CL_Vec2f(texCoordHelper.left, texCoordHelper.bottom)
    };


    if (lastTexture_ != tex) {
        batchFlush(gc);
        lastTexture_ = tex;
    }

    memcpy(&batchPositions_[batchFill_], curObj->getVertexCoordinates(), sizeof(CL_Vec3f) * 6);
    memcpy(&batchNormals_[batchFill_], curObj->getVertexNormals(), sizeof(CL_Vec3f) * 6);

    if (curObj->isMirrored()) {
        memcpy(&batchTexCoords_[batchFill_], texCoordsMirrored, sizeof(CL_Vec2f) * 6);
    } else {
        memcpy(&batchTexCoords_[batchFill_], texCoords, sizeof(CL_Vec2f) * 6);
    }

    float depth = curObj->getRenderDepth() - minRenderDepth;
    depth = depth / renderDiff;
    CL_Vec3f hueInfo = curObj->getHueInfo();
    for (unsigned int i = 0; i < 6; ++i) {
        batchHueInfos_[batchFill_ + i] = hueInfo;
        batchPositions_[batchFill_ + i].z = depth;
    }

    //LOG_DEBUG << "depth: " << batchPositions_[batchFill_].z << std::endl;

    batchFill_ += 6;

    if (batchFill_ == BATCH_NUM_VERTICES) {
        batchFlush(gc);
    }
}

void WorldViewRenderer::batchFlush(CL_GraphicContext& gc) {
    if (batchFill_ > 0 && lastTexture_) {
        //LOG_DEBUG << "batch flush: " << (batchFill_ / 6) << std::endl;
        CL_PrimitivesArray primarray(gc);
        primarray.set_attributes(0, batchPositions_);
        primarray.set_attributes(1, batchTexCoords_);
        primarray.set_attributes(2, batchNormals_);
        primarray.set_attributes(3, batchHueInfos_);

        gc.set_texture(1, *lastTexture_->getTexture());

        gc.draw_primitives(cl_triangles, batchFill_, primarray);

        batchFill_ = 0;
    }
}

}
}
