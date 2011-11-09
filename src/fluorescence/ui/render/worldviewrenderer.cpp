
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
    }
}

boost::shared_ptr<Texture> WorldViewRenderer::getTexture(CL_GraphicContext& gc) {
    if (renderQueue_->requireWorldRepaint() || !texture_ || requireInitialRepaint()) {
        checkTextureSize();
        CL_FrameBuffer origBuffer = gc.get_write_frame_buffer();

        CL_FrameBuffer fb(gc);
        fb.attach_color_buffer(0, *texture_->getTexture());

        gc.set_frame_buffer(fb);

        render(gc);

        gc.set_frame_buffer(origBuffer);
    }

    return texture_;
}


void WorldViewRenderer::render(CL_GraphicContext& gc) {
    renderQueue_->preRender();


    gc.clear(CL_Colorf(0.f, 0.f, 0.f, 1.f));

    gc.set_program_object(*shaderProgram_, cl_program_matrix_modelview_projection);

    CL_Rectf texture_unit1_coords(0.0f, 0.0f, 1.0f, 1.0f);

    CL_Vec2f tex1_coords[6] = {
        CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.top),
        CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
        CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
        CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
        CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
        CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.bottom)
    };

    CL_Vec2f tex1_coords_mirrored[6] = {
        CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
        CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.top),
        CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.bottom),
        CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.top),
        CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.bottom),
        CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom)
    };

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

    for (; igIter != igEnd; ++igIter) {
        boost::shared_ptr<world::IngameObject> curObj = *igIter;

        // object is invisible
        if (!curObj->isVisible()) {
            continue;
        }

        // check if texture is ready to be drawn
        boost::shared_ptr<ui::Texture> tex = curObj->getIngameTexture();

        // happens e.g. for the equipped backpack
        if (!tex) {
            continue;
        }

        if (!tex->isReadComplete()) {
            renderingComplete = false;
            continue;
        }

        // check if current object is in the area visible to the player
        if (!curObj->isInDrawArea(clippingLeftPixelCoord, clippingRightPixelCoord, clippingTopPixelCoord, clippingBottomPixelCoord)) {
            continue;
        }

        CL_PrimitivesArray primarray(gc);
        primarray.set_attributes(0, curObj->getVertexCoordinates());
        if (curObj->isMirrored()) {
            primarray.set_attributes(1, tex1_coords_mirrored);
        } else {
            primarray.set_attributes(1, tex1_coords);
        }
        primarray.set_attributes(2, curObj->getVertexNormals());

        primarray.set_attribute(3, curObj->getHueInfo());

        gc.set_texture(1, *tex->getTexture());
        gc.draw_primitives(cl_triangles, 6, primarray);
    }

    gc.reset_textures();
    gc.reset_program_object();

    renderQueue_->postRender(renderingComplete);
}

boost::shared_ptr<RenderQueue> WorldViewRenderer::getRenderQueue() const {
    return renderQueue_;
}

}
}
