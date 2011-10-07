
#include "ingameviewrenderer.hpp"

#include "ingameview.hpp"

#include <ui/manager.hpp>
#include <ui/renderqueue.hpp>
#include <ui/texture.hpp>
#include <ui/fontengine.hpp>

#include <misc/log.hpp>

#include <world/ingameobject.hpp>
#include <world/lightmanager.hpp>
#include <world/manager.hpp>

#include <data/manager.hpp>
#include <data/huesloader.hpp>

namespace fluo {
namespace ui {

IngameViewRenderer::IngameViewRenderer(IngameView* ingameView) :
        ingameView_(ingameView) {

    CL_GraphicContext gc = fluo::ui::Manager::getSingleton()->getGraphicContext();

    shaderProgram_.reset(new CL_ProgramObject(CL_ProgramObject::load(gc, "shader/vertex.glsl", "shader/fragment.glsl")));
    shaderProgram_->bind_attribute_location(0, "gl_Vertex");
    shaderProgram_->bind_attribute_location(1, "TexCoord0");
    shaderProgram_->bind_attribute_location(2, "gl_Normal");
    shaderProgram_->bind_attribute_location(3, "HueInfo0");

    if (!shaderProgram_->link()) {
        LOG_EMERGENCY << "Error while linking program:\n" << shaderProgram_->get_info_log().c_str() << std::endl;
        throw CL_Exception("Unable to link program");
    }
}

IngameViewRenderer::~IngameViewRenderer() {
}

void IngameViewRenderer::renderOneFrame(CL_GraphicContext& gc, const CL_Rect& clipRect) {
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

    CL_Vec2f tex1_coords_mirrored[6] = {
        CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
        CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.top),
        CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.bottom),
        CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.top),
        CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.bottom),
        CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom)
    };

    int clippingLeftPixelCoord = ingameView_->getCenterPixelX() - ingameView_->getWidth()/2;
    int clippingRightPixelCoord = ingameView_->getCenterPixelX() + ingameView_->getWidth()/2;
    int clippingTopPixelCoord = ingameView_->getCenterPixelY() - ingameView_->getHeight()/2;
    int clippingBottomPixelCoord = ingameView_->getCenterPixelY() + ingameView_->getHeight()/2;


    CL_Vec2f pixelOffsetVec(clippingLeftPixelCoord, clippingTopPixelCoord);
    shaderProgram_->set_uniform2f("PositionOffset", pixelOffsetVec);

    gc.set_texture(0, *(data::Manager::getSingleton()->getHuesLoader()->getHuesTexture()->getTexture()));
    shaderProgram_->set_uniform1i("HueTexture", 0);
    shaderProgram_->set_uniform1i("ObjectTexture", 1);

    boost::shared_ptr<world::LightManager> lightManager = world::Manager::getSingleton()->getLightManager();
    shaderProgram_->set_uniform3f("AmbientLightIntensity", lightManager->getAmbientIntensity());
    shaderProgram_->set_uniform3f("GlobalLightIntensity", lightManager->getGlobalIntensity());
    shaderProgram_->set_uniform3f("GlobalLightDirection", lightManager->getGlobalDirection());

    boost::shared_ptr<RenderQueue> renderQueue = ui::Manager::getSingleton()->getRenderQueue();
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

        if (!tex || !tex->isReadComplete()) {
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



    //boost::shared_ptr<ui::Texture> testText = data::Manager::getUnicodeText(0, "abcdefghijklmnopqrstuvwxyz1234567890", true, 140, 0xFF0000FF);
    //CL_Draw::texture(gc, *testText->getTexture(), CL_Rectf(0, 0, CL_Sizef(testText->getWidth(), testText->getHeight())));

    //boost::shared_ptr<ui::Texture> testText2 = data::Manager::getUnicodeText(1, "foobar omg mäh superlanger text", true, 140, 0xFF0000FF);
    //CL_Draw::texture(gc, *testText2->getTexture(), CL_Rectf(0, 40, CL_Sizef(testText2->getWidth(), testText2->getHeight())));

    boost::shared_ptr<ui::Texture> testText3 = ui::Manager::getFontEngine()->getUniFontTexture(1, "The quick brown fox jumps over the lazy dog wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww", 140, 0x00FF00FF, 1);
    CL_Draw::texture(gc, *testText3->getTexture(), CL_Rectf(30, 120, CL_Sizef(testText3->getWidth(), testText3->getHeight())));


    boost::shared_ptr<ui::Texture> textTexture = ui::Manager::getFontEngine()->getDefaultTexture("The quick brown fox jumps over the lazy dog", 140, 0xFFFF00FF, 1);
    CL_Draw::texture(gc, *textTexture->getTexture(), CL_Rectf(30, 30, CL_Sizef(textTexture->getWidth(), textTexture->getHeight())));
}

}
}
