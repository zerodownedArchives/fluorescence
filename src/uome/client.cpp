
#include "client.hpp"

#include "config.hpp"

#include <ui/manager.hpp>
#include <ui/texture.hpp>

#include <data/manager.hpp>


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
        return 1;
    }

    LOG_INFO(LOGTYPE_MAIN, "Initializing ui");
    if (!ui::Manager::create(config_)) {
        return 1;
    }




    CL_DisplayWindow* wnd = uome::ui::Manager::getSingleton()->getWindow();

    boost::shared_ptr<uome::ui::Texture> myTex = uome::data::Manager::getArtLoader()->getMapTexture(4);
    boost::shared_ptr<uome::ui::Texture> myTex2 = uome::data::Manager::getArtLoader()->getItemTexture(10);
    boost::shared_ptr<uome::ui::Texture> myTex3 = uome::data::Manager::getGumpArtLoader()->getTexture(100);


    for (unsigned int i = 0; i < 500; ++i) {
        CL_GraphicContext gc = uome::ui::Manager::getSingleton()->getGC();
        gc.clear(CL_Colorf(0.0f, 0.0f, 0.0f));
        // CL_Draw::fill(gc, CL_Rectf(240.0f, 140.0f, 440.0f, 340.0f), CL_Colorf(1.0f, 1.0f, 1.0f));
        if (myTex->isReadComplete()) {
            gc.draw_pixels(20, 20, *(myTex->getPixelBuffer()), CL_Rect(0, 0, 44, 44));
            gc.draw_pixels(64, 64, *(myTex->getPixelBuffer()), CL_Rect(0, 0, 44, 44));
            gc.draw_pixels(20, 64, *(myTex->getPixelBuffer()), CL_Rect(0, 0, 44, 44));
            gc.draw_pixels(64, 20, *(myTex->getPixelBuffer()), CL_Rect(0, 0, 44, 44));
            gc.draw_pixels(42, 42, *(myTex->getPixelBuffer()), CL_Rect(0, 0, 44, 44));
        }

        if (myTex2->isReadComplete()) {
            gc.draw_pixels(230, 230, *(myTex2->getPixelBuffer()), CL_Rect(0, 0, myTex2->getPixelBuffer()->get_width(), myTex2->getPixelBuffer()->get_height()));
        }

        if (myTex3->isReadComplete()) {
            gc.draw_pixels(400, 30, *(myTex3->getPixelBuffer()), CL_Rect(0, 0, myTex3->getPixelBuffer()->get_width(), myTex3->getPixelBuffer()->get_height()));
        }

        wnd->flip();
        CL_KeepAlive::process();
        CL_System::sleep(10);
    }

    return 0;
}

}
