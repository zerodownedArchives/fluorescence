
#include "client.hpp"

#include "config.hpp"

#include <ui/manager.hpp>
#include <ui/texture.hpp>

#include <data/manager.hpp>
#include <data/artloader.hpp>
#include <data/gumpartloader.hpp>
#include <data/maploader.hpp>
#include <data/staticsloader.hpp>
#include <data/maptexloader.hpp>

#include <world/sector.hpp>
#include <world/map.hpp>
#include <world/statics.hpp>

#include <iostream>
#include <iomanip>

namespace uome {

Client::Client() {
}

int Client::sMain(const std::vector<CL_String8>& args) {
    Client instance;
    return instance.main(args);
}

void printUint(unsigned int i) {
    printf("printUint: %u\n", i);
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

    int pixelOffsetX = -5000;
    int pixelOffsetY = 66500;

    for (unsigned int i = 1; !ic.get_keyboard().get_keycode(CL_KEY_ESCAPE); ++i) {
        if (i % 50 == 0) {
            timeval curTime;
            gettimeofday(&curTime, NULL);

            float diff = (curTime.tv_sec - lastTime.tv_sec);
            diff += (curTime.tv_usec - lastTime.tv_usec) / 1000000.0f;

            float fps = diff / 50.0f;
            fps = 1/fps;

            std::ostringstream titleHelper;
            titleHelper << "UO:ME -- fps: " << std::setiosflags(std::ios::fixed) << std::setprecision(1) << fps;
            wnd->set_title(titleHelper.str());
            LOGARG_DEBUG(LOGTYPE_MAIN, "fps: %.1f", fps);
            lastTime = curTime;
        }

        if (ic.get_keyboard().get_keycode(CL_KEY_DOWN)) {
            pixelOffsetY += 20;
        } else if (ic.get_keyboard().get_keycode(CL_KEY_UP)) {
            pixelOffsetY -= 20;
        }

        if (ic.get_keyboard().get_keycode(CL_KEY_LEFT)) {
            pixelOffsetX -= 20;
        } else if (ic.get_keyboard().get_keycode(CL_KEY_RIGHT)) {
            pixelOffsetX += 20;
        }

        int leftPixelCoord = pixelOffsetX;
        int rightPixelCoord = pixelOffsetX + wnd->get_viewport().get_width();
        int topPixelCoord = pixelOffsetY;
        int bottomPixelCoord = pixelOffsetY + wnd->get_viewport().get_height();
        CL_Vec2f pixelOffsetVec(pixelOffsetX, pixelOffsetY);


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

        ui::RenderQueue* renderQueue = ui::Manager::getSingleton()->getRenderQueue();
        renderQueue->prepareRender();

        std::list<world::IngameObject*>::iterator igIter = renderQueue->beginIngame();
        std::list<world::IngameObject*>::iterator igEnd = renderQueue->endIngame();

        for (; igIter != igEnd; ++igIter) {
            world::IngameObject* curObj = *igIter;

            // object has invisible property set
            if (!curObj->isVisible()) {
                continue;
            }

            // check if texture is ready to be drawn
            boost::shared_ptr<ui::Texture> tex = curObj->getIngameTexture();
            if (!tex->isReadComplete()) {
                continue;
            }

            if (!curObj->isRenderDataValid()) {
                curObj->updateRenderData();
            }

            // check if current object is in the area visible to the player
            if (!curObj->isInDrawArea(leftPixelCoord, rightPixelCoord, topPixelCoord, bottomPixelCoord)) {
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

        wnd->flip();
        CL_KeepAlive::process();
        //CL_System::sleep(10);
    }

    return 0;
}

}
