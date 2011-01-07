
#include "ingamewindowrenderer.hpp"

#include "manager.hpp"
#include "renderqueue.hpp"
#include "texture.hpp"
#include "ingamewindow.hpp"

#include <misc/logger.hpp>

#include <world/ingameobject.hpp>

namespace uome {
namespace ui {

IngameWindowRenderer::IngameWindowRenderer(IngameWindow* ingameWindow) :
        ingameWindow_(ingameWindow) {

    CL_GraphicContext gc = uome::ui::Manager::getSingleton()->getGraphicsContext();

    shaderProgram_.reset(new CL_ProgramObject(CL_ProgramObject::load(gc, "../shader/vertex.glsl", "../shader/fragment.glsl")));
    shaderProgram_->bind_attribute_location(0, "Position");
    shaderProgram_->bind_attribute_location(1, "TexCoord0");
    if (!shaderProgram_->link())
        throw CL_Exception("Unable to link program");
}

IngameWindowRenderer::~IngameWindowRenderer() {
}

void IngameWindowRenderer::renderOneFrame(CL_GraphicContext& gc, const CL_Rect& clipRect) {
    RenderQueue* renderQueue = uome::ui::Manager::getSingleton()->getRenderQueue();

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

    int clippingLeftPixelCoord = ingameWindow_->getCenterPixelX() - ingameWindow_->getWidth()/2;
    int clippingRightPixelCoord = ingameWindow_->getCenterPixelX() + ingameWindow_->getWidth()/2;
    int clippingTopPixelCoord = ingameWindow_->getCenterPixelY() - ingameWindow_->getHeight()/2;
    int clippingBottomPixelCoord = ingameWindow_->getCenterPixelY() + ingameWindow_->getHeight()/2;


    CL_Vec2f pixelOffsetVec(clippingLeftPixelCoord, clippingTopPixelCoord);


    std::list<world::IngameObject*>::const_iterator igIter = renderQueue->beginIngame();
    std::list<world::IngameObject*>::const_iterator igEnd = renderQueue->endIngame();

    for (; igIter != igEnd; ++igIter) {
        world::IngameObject* curObj = *igIter;

        // object is invisible
        if (!curObj->isVisible()) {
            continue;
        }

        // check if texture is ready to be drawn
        boost::shared_ptr<ui::Texture> tex = curObj->getIngameTexture();
        if (!tex->isReadComplete()) {
            continue;
        }

        // check if current object is in the area visible to the player
        if (!curObj->isInDrawArea(clippingLeftPixelCoord, clippingRightPixelCoord, clippingTopPixelCoord, clippingBottomPixelCoord)) {
            continue;
        }

        CL_PrimitivesArray primarray(gc);
        primarray.set_attributes(0, curObj->getVertexCoordinates());
        primarray.set_attributes(1, tex1_coords);
        primarray.set_attribute(2, pixelOffsetVec);

        gc.set_texture(0, *tex->getTexture());
        gc.draw_primitives(cl_triangles, 6, primarray);
        gc.reset_texture(0);
    }

    gc.reset_program_object();
}

}
}
