
#include "client.hpp"

#include "config.hpp"

#include <ui/manager.hpp>
#include <ui/texture.hpp>

#include <data/manager.hpp>
#include <data/artloader.hpp>
#include <data/gumpartloader.hpp>
#include <data/maploader.hpp>
#include <data/staticsloader.hpp>

#include <world/sector.hpp>
#include <world/map.hpp>
#include <world/statics.hpp>

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


    std::list<boost::shared_ptr<uome::world::Sector> > sectorList;
    for (unsigned int j = 200; j < 210; ++j) {
        for (unsigned int i = 170; i < 180; ++i) {
            sectorList.push_back(boost::shared_ptr<uome::world::Sector>(new uome::world::Sector(i * 512 + j, i, j)));
        }
    }

    CL_GraphicContext gc = uome::ui::Manager::getSingleton()->getGC();
    CL_InputContext ic = uome::ui::Manager::getSingleton()->getIC();


    CL_ProgramObject program = CL_ProgramObject::load(gc, "../shader/vertex.glsl", "../shader/fragment.glsl");
    program.bind_attribute_location(0, "Position");
    program.bind_attribute_location(1, "TexCoord0");
    if (!program.link())
        throw CL_Exception("Unable to link program");


    timeval lastTime;
    gettimeofday(&lastTime, NULL);

    float posOffsetX = 5000;
    float posOffsetY = -66500;

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

        if (ic.get_keyboard().get_keycode(CL_KEY_DOWN)) {
            posOffsetY -= 50;
        } else if (ic.get_keyboard().get_keycode(CL_KEY_UP)) {
            posOffsetY += 50;
        }

        if (ic.get_keyboard().get_keycode(CL_KEY_LEFT)) {
            posOffsetX += 50;
        } else if (ic.get_keyboard().get_keycode(CL_KEY_RIGHT)) {
            posOffsetX -= 50;
        }

        gc.clear(CL_Colorf(0.0f, 0.0f, 0.0f));


        gc.set_program_object(program, cl_program_matrix_modelview_projection);

        CL_Rectf texture_unit1_coords(0.0f, 0.0f, 1.0f, 1.0f);

        CL_Vec2f tex1_coords[6] = {
            CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.top),
            CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
            CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
            CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
            CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
            CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.bottom)
        };

        std::list<boost::shared_ptr<uome::world::Sector> >::iterator iter, end;
        iter = sectorList.begin();
        end = sectorList.end();
        for (iter = sectorList.begin(); iter != end; ++iter) {
            boost::shared_ptr<uome::world::Sector> sector = *iter;
            boost::shared_ptr<uome::world::MapBlock> mapBlock = sector->getMapBlock();
            boost::shared_ptr<uome::world::StaticBlock> staticBlock = sector->getStaticBlock();

            if (mapBlock->isReadComplete()) {
                for (unsigned int x = 0; x < 8; ++x) {
                    for (unsigned int y = 0; y < 8; ++y) {
                        world::MapTile* tile = mapBlock->get(x, y);
                        boost::shared_ptr<ui::Texture> tex = tile->getIngameTexture();
                        if (tex->isReadComplete()) {
                            if (!tile->isRenderDataValid()) {
                                tile->updateRenderData();
                            }

                            CL_PrimitivesArray primarray(gc);
                            primarray.set_attributes(0, tile->getVertexCoordinates());
                            primarray.set_attributes(1, tex1_coords);
                            CL_Vec2f posOffset(posOffsetX, posOffsetY);
                            primarray.set_attribute(2, posOffset);

                            gc.set_texture(0, *(tex->getTexture()));
                            gc.draw_primitives(cl_triangles, 6, primarray);
                            //gc.reset_texture(0);

                            //gc.draw_pixels(pxX, pxY, *(tex->getPixelBuffer()), CL_Rect(0, 0, 44, 44));
                        }
                    }
                }
            }


            if (staticBlock->isReadComplete()) {
                std::list<boost::shared_ptr<world::StaticItem> > lst = staticBlock->getItemList();
                //LOGARG_DEBUG(LOGTYPE_MAIN, "Item count in static list: %u", lst.size());

                std::list<boost::shared_ptr<world::StaticItem> >::iterator iter = lst.begin();
                std::list<boost::shared_ptr<world::StaticItem> >::iterator end = lst.end();
                for (; iter != end; ++iter) {
                    boost::shared_ptr<world::StaticItem> itm = *iter;
                    boost::shared_ptr<ui::Texture> tex = itm->getIngameTexture();

                    if (tex->isReadComplete()) {
                        if (!itm->isRenderDataValid()) {
                            itm->updateRenderData();
                        }

                        //LOGARG_DEBUG(LOGTYPE_MAIN, "drawing item fact=%u art=%u at x=%u y=%u pxX=%u pxY=%u width=%u height=%u", fact, itm->getArtId(), x, y, pxX, pxY, tex->getWidth(), tex->getHeight());


                        CL_PrimitivesArray primarray(gc);
                        primarray.set_attributes(0, itm->getVertexCoordinates());
                        primarray.set_attributes(1, tex1_coords);
                        CL_Vec2f posOffset(posOffsetX, posOffsetY);
                        primarray.set_attribute(2, posOffset);

                        gc.set_texture(0, *(tex->getTexture()));
                        gc.draw_primitives(cl_triangles, 6, primarray);
                        //gc.reset_texture(0);

                        //gc.draw_pixels(pxX, pxY, *(tex->getPixelBuffer()), CL_Rectf(pxX, pxY, tex->getWidth(), tex->getHeight()));
                        //CL_Draw::texture(gc, *(tex->getTexture()), CL_Rectf(0, 0, tex->getWidth(), tex->getHeight()));
                    }
                }
            }
        }

        gc.reset_program_object();

        wnd->flip();
        CL_KeepAlive::process();
        //CL_System::sleep(10);
    }

    return 0;
}

}
