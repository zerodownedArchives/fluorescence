
#include "client.hpp"

#include "config.hpp"

#include <ui/manager.hpp>
#include <ui/texture.hpp>

#include <data/manager.hpp>
#include <data/artloader.hpp>
#include <data/gumpartloader.hpp>
#include <data/maploader.hpp>

#include <world/map.hpp>

namespace uome {

Client::Client() {
}

int Client::sMain(const std::vector<CL_String8>& args) {
    Client instance;
    return instance.main(args);
}

int Client::main(const std::vector<CL_String8>& args) {
    LOG_INFO(LOGTYPE_MAIN, "Starting client");

    LOG_INFO(LOGTYPE_MAIN, "Parsing configuration");
    if (!Config::getVariablesMap(args, config_)) {
        return 1;
    }

    LOG_INFO(LOGTYPE_MAIN, "Creating data loaders");
    if (!data::Manager::create(config_)) {
        LOG_DEBUG(LOGTYPE_MAIN, "Unable to intialize manager, exiting!");
        return 1;
    }

    LOG_INFO(LOGTYPE_MAIN, "Initializing ui");
    if (!ui::Manager::create(config_)) {
        return 1;
    }




    CL_DisplayWindow* wnd = uome::ui::Manager::getSingleton()->getWindow();

    boost::shared_ptr<uome::ui::Texture> myTex2 = uome::data::Manager::getArtLoader()->getItemTexture(10);
    boost::shared_ptr<uome::ui::Texture> myTex3 = uome::data::Manager::getGumpArtLoader()->getTexture(100);

    boost::shared_ptr<uome::world::MapBlock> myBlock = uome::data::Manager::getMapLoader()->get(0, 0);


    CL_GraphicContext gc = uome::ui::Manager::getSingleton()->getGC();
    CL_InputContext ic = uome::ui::Manager::getSingleton()->getIC();


    CL_ProgramObject program = CL_ProgramObject::load(gc, "../shader/vertex.glsl", "../shader/fragment.glsl");
    program.bind_attribute_location(0, "Position");
    program.bind_attribute_location(1, "TexCoord0");
    if (!program.link())
        throw CL_Exception("Unable to link program");


    timeval lastTime;
    gettimeofday(&lastTime, NULL);

    for (unsigned int i = 1; !ic.get_keyboard().get_keycode(CL_KEY_ESCAPE); ++i) {
        if (i % 50 == 0) {
            timeval curTime;
            gettimeofday(&curTime, NULL);

            float diff = (curTime.tv_sec - lastTime.tv_sec);
            diff += (curTime.tv_usec - lastTime.tv_usec) / 1000000.0f;

            float fps = diff / 50.0f;
            fps = 1/fps;

            LOGARG_DEBUG(LOGTYPE_MAIN, "fps: %.1f", fps);
            lastTime = curTime;
        }

        gc.clear(CL_Colorf(0.0f, 0.0f, 0.0f));


        if (myTex3->isReadComplete()) {
            gc.draw_pixels(400, 30, *(myTex3->getPixelBuffer()), CL_Rect(0, 0, myTex3->getPixelBuffer()->get_width(), myTex3->getPixelBuffer()->get_height()));
        }

        gc.set_program_object(program, cl_program_matrix_modelview_projection);

        if (myBlock->isReadComplete()) {
            for (unsigned int fact = 1; fact < 30; ++fact) {
                for (unsigned int x = 0; x < 8; ++x) {
                    for (unsigned int y = 0; y < 8; ++y) {
                        const world::MapTile tile = myBlock->get(x, y);
                        boost::shared_ptr<ui::Texture> tex = tile.getIngameTexture();
                        if (tex->isReadComplete()) {
                            unsigned int pxX = fact * 176 + 50;
                            unsigned int pxY = fact * 176 + 50;

                            pxX += (x * 22);
                            pxY += (x * 22);

                            pxX -= (y * 22);
                            pxY += (y * 22);


                            CL_Rectf rect(pxX, pxY, pxX + 44, pxY + 44);
                            CL_Rectf texture_unit1_coords(0.0f, 0.0f, 1.0f, 1.0f);

                            CL_Vec2f positions[6] =
                            {
                                CL_Vec2f(rect.left, rect.top),
                                CL_Vec2f(rect.right, rect.top),
                                CL_Vec2f(rect.left, rect.bottom),
                                CL_Vec2f(rect.right, rect.top),
                                CL_Vec2f(rect.left, rect.bottom),
                                CL_Vec2f(rect.right, rect.bottom)
                            };

                            CL_Vec2f tex1_coords[6] =
                            {
                                CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.top),
                                CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
                                CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
                                CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
                                CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
                                CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.bottom)
                            };

                            CL_PrimitivesArray primarray(gc);
                            primarray.set_attributes(0, positions);
                            primarray.set_attributes(1, tex1_coords);

                            gc.set_texture(0, *(tex->getTexture()));
                            gc.draw_primitives(cl_triangles, 6, primarray);
                            gc.reset_texture(0);

                            //gc.draw_pixels(pxX, pxY, *(tex->getPixelBuffer()), CL_Rect(0, 0, 44, 44));
                        }
                    }
                }
            }
        }

        gc.reset_program_object();

        if (myTex2->isReadComplete()) {
            gc.draw_pixels(230, 230, *(myTex2->getPixelBuffer()), CL_Rect(0, 0, myTex2->getPixelBuffer()->get_width(), myTex2->getPixelBuffer()->get_height()));
        }

        wnd->flip();
        CL_KeepAlive::process();
        //CL_System::sleep(10);
    }

    return 0;
}

}
