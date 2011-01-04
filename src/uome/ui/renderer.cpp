
#include "renderer.hpp"

#include "manager.hpp"
#include "renderqueue.hpp"

#include <logger.hpp>

#include <ui/texture.hpp>

#include <world/ingameobject.hpp>

namespace uome {
namespace ui {

Renderer::Renderer(CL_GraphicContext& gc) :
        initialized_(false),
        ingameClippingLeftPixelCoord_(0),
        ingameClippingRightPixelCoord_(0),
        ingameClippingTopPixelCoord_(0),
        ingameClippingBottomPixelCoord_(0) {

    renderQueue_.reset(new RenderQueue());

    shaderProgram_.reset(new CL_ProgramObject(CL_ProgramObject::load(gc, "../shader/vertex.glsl", "../shader/fragment.glsl")));
    shaderProgram_->bind_attribute_location(0, "Position");
    shaderProgram_->bind_attribute_location(1, "TexCoord0");
    if (!shaderProgram_->link())
        throw CL_Exception("Unable to link program");
}

Renderer::~Renderer() {
}

void Renderer::initialize() {
    initialized_ = true;
}

void Renderer::setIngameClipping(int leftPixelCoord, int rightPixelCoord, int topPixelCoord, int bottomPixelCoord) {
    ingameClippingLeftPixelCoord_ = leftPixelCoord;
    ingameClippingRightPixelCoord_ = rightPixelCoord;
    ingameClippingTopPixelCoord_ = topPixelCoord;
    ingameClippingBottomPixelCoord_ = bottomPixelCoord;
}

void Renderer::renderOneFrame() {
    if (!initialized_) {
        initialize();
    }

    CL_GraphicContext gc = uome::ui::Manager::getSingleton()->getGC();

    CL_Vec2f pixelOffsetVec(ingameClippingLeftPixelCoord_, ingameClippingTopPixelCoord_);


    gc.clear(CL_Colorf(0.0f, 0.0f, 0.0f));

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

    renderQueue_->prepareRender();


    std::list<world::IngameObject*>::iterator igIter = renderQueue_->beginIngame();
    std::list<world::IngameObject*>::iterator igEnd = renderQueue_->endIngame();

    for (; igIter != igEnd; ++igIter) {
        world::IngameObject* curObj = *igIter;

        // object is invisible
        if (!curObj->isVisible()) {
            continue;
        }

        // update rendering data (priority, vertex coordinates, texture, ...)
        if (!curObj->isRenderDataValid()) {
            curObj->updateRenderData();
        }

        // check if texture is ready to be drawn
        boost::shared_ptr<ui::Texture> tex = curObj->getIngameTexture();
        if (!tex->isReadComplete()) {
            continue;
        }

        // check if current object is in the area visible to the player
        if (!curObj->isInDrawArea(ingameClippingLeftPixelCoord_, ingameClippingRightPixelCoord_, ingameClippingTopPixelCoord_, ingameClippingBottomPixelCoord_)) {
            continue;
        }

        CL_PrimitivesArray primarray(gc);
        primarray.set_attributes(0, curObj->getVertexCoordinates());
        primarray.set_attributes(1, tex1_coords);
        primarray.set_attribute(2, pixelOffsetVec);

        gc.set_texture(0, *(tex->getTexture()));
        gc.draw_primitives(cl_triangles, 6, primarray);
        gc.reset_texture(0);
    }

    gc.reset_program_object();
}

boost::shared_ptr<RenderQueue> Renderer::getRenderQueue() {
    return renderQueue_;
}

}
}
