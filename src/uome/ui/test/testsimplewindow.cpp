
#include <ui/manager.hpp>
#include <data/artloader.hpp>

#include <ui/texture.hpp>

#include <boost/shared_ptr.hpp>

int myMain(const std::vector<CL_String> &args) {
    int id = 4;
    int id2 = 4;
    if (args.size() > 1) {
        id = atoi(args[1].c_str());
    }

    if (args.size() > 2) {
        id2 = atoi(args[2].c_str());
    }

    printf("creating window\n");
    CL_DisplayWindow* wnd = uome::ui::Manager::getSingleton()->getWindow();

    printf("loading texture\n");

    uome::data::ArtLoader loader("/data/games/uo2d/artidx.mul", "/data/games/uo2d/art.mul");

    printf("enqueue\n");
    boost::shared_ptr<uome::ui::Texture> myTex = loader.getMapTexture(id);
    boost::shared_ptr<uome::ui::Texture> myTex2 = loader.getItemTexture(id2);
    printf("after sleep");

    //printf("create tex\n");
    //boost::shared_ptr<CL_Texture> tex = uome::ui::Manager::getSingleton()->provideTexture(44, 44);
    //printf("set image\n");
    //tex->set_image(*(myTex->getPixelBuffer()));
    //printf("post set image\n");

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
        wnd->flip();
        CL_KeepAlive::process();
        CL_System::sleep(10);
    }

    return 0;
}

CL_ClanApplication app(&myMain);
