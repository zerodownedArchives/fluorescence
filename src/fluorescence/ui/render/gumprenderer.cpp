
#include "gumprenderer.hpp"

#include <ui/manager.hpp>
#include <ui/render/renderqueue.hpp>
#include <ui/texture.hpp>
#include <ui/components/gumpview.hpp>

#include <misc/log.hpp>

#include <world/ingameobject.hpp>

#include <data/manager.hpp>
#include <data/huesloader.hpp>

namespace fluo {
namespace ui {

GumpRenderer::GumpRenderer(boost::shared_ptr<RenderQueue> renderQueue, components::GumpView* gumpView) : IngameObjectRenderer(IngameObjectRenderer::TYPE_GUMP, true),
            gumpView_(gumpView), renderQueue_(renderQueue) {

    CL_GraphicContext gc = fluo::ui::Manager::getSingleton()->getGraphicContext();

    shaderProgram_.reset(new CL_ProgramObject(CL_ProgramObject::load(gc, "shader/gump_vertex.glsl", "shader/gump_fragment.glsl")));
    shaderProgram_->bind_attribute_location(0, "gl_Vertex");
    shaderProgram_->bind_attribute_location(1, "TexCoord0");
    shaderProgram_->bind_attribute_location(2, "HueInfo0");

    if (!shaderProgram_->link()) {
        LOG_EMERGENCY << "Error while linking program:\n" << shaderProgram_->get_info_log().c_str() << std::endl;
        throw CL_Exception("Unable to link program");
    }
}

GumpRenderer::~GumpRenderer() {
    renderQueue_->clear();
}


void GumpRenderer::checkTextureSize() {
    if (!texture_ || texture_->getWidth() != gumpView_->getWidth() || texture_->getHeight() != gumpView_->getHeight()) {
        texture_.reset(new ui::Texture(false));
        texture_->initPixelBuffer(gumpView_->getWidth(), gumpView_->getHeight());
        texture_->setReadComplete();
    }
}

boost::shared_ptr<Texture> GumpRenderer::getTexture(CL_GraphicContext& gc) {
    if (renderQueue_->requireGumpRepaint() || !texture_) {
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


void GumpRenderer::render(CL_GraphicContext& gc) {
    renderQueue_->preRender();

    gc.clear(CL_Colorf(0.f, 0.f, 0.f, 0.f));

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

    gc.set_texture(0, *(data::Manager::getSingleton()->getHuesLoader()->getHuesTexture()->getTexture()));
    shaderProgram_->set_uniform1i("HueTexture", 0);
    shaderProgram_->set_uniform1i("ObjectTexture", 1);

    RenderQueue::const_iterator igIter = renderQueue_->begin();
    RenderQueue::const_iterator igEnd = renderQueue_->end();

    CL_Vec2f vertexCoords[6];

    bool renderingComplete = true;

    for (; igIter != igEnd; ++igIter) {
        boost::shared_ptr<world::IngameObject> curObj = *igIter;

        // just mobiles and items in a gump
        if (!curObj->isMobile() && ! curObj->isDynamicItem()) {
            continue;
        }

        // check if texture is ready to be drawn
        boost::shared_ptr<ui::Texture> tex = curObj->getGumpTexture();

        if (!tex || !tex->isReadComplete()) {
            renderingComplete = false;
            continue;
        }

        CL_Rectf rect(0, 0, tex->getWidth(), tex->getHeight());

        vertexCoords[0] = CL_Vec2f(rect.left, rect.top);
        vertexCoords[1] = CL_Vec2f(rect.right, rect.top);
        vertexCoords[2] = CL_Vec2f(rect.left, rect.bottom);
        vertexCoords[3] = CL_Vec2f(rect.right, rect.top);
        vertexCoords[4] = CL_Vec2f(rect.left, rect.bottom);
        vertexCoords[5] = CL_Vec2f(rect.right, rect.bottom);

        CL_PrimitivesArray primarray(gc);
        primarray.set_attributes(0, vertexCoords);
        primarray.set_attributes(1, tex1_coords);

        primarray.set_attribute(2, curObj->getHueInfo());

        gc.set_texture(1, *tex->getTexture());
        gc.draw_primitives(cl_triangles, 6, primarray);
    }

    gc.reset_textures();
    gc.reset_program_object();

    renderQueue_->postRender(renderingComplete);
}

boost::shared_ptr<RenderQueue> GumpRenderer::getRenderQueue() const {
    return renderQueue_;
}

}
}
