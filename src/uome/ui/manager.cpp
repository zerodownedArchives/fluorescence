
#include "manager.hpp"

#include <boost/filesystem/path.hpp>

#include "ingamewindow.hpp"
#include "renderqueue.hpp"

#include <misc/logger.hpp>
#include <misc/exception.hpp>

namespace uome {
namespace ui {

Manager* Manager::singleton_ = NULL;

bool Manager::create(const boost::program_options::variables_map& config) {
    if (!singleton_) {
        try {
            singleton_ = new Manager(config);
        } catch (const std::exception& ex) {
            LOGARG_CRITICAL(LOGTYPE_UI, "Error initializing ui::Manager: %s", ex.what());
            return false;
        }
    }

    return true;
}


void Manager::destroy() {
    if (singleton_) {
        delete singleton_;
        singleton_ = NULL;
    }
}

Manager* Manager::getSingleton() {
    if (!singleton_) {
        throw Exception("uome::ui::Manager Singleton called before being created");
    }

    return singleton_;
}

Manager::Manager(const boost::program_options::variables_map& config) {
    CL_SetupCore clSetupCore;
    CL_SetupDisplay clSetupDisplay;
    CL_SetupGL clSetupGL;
    CL_SetupGUI clSetupGUI;

    CL_OpenGLWindowDescription description;
    description.set_size(CL_Size(800, 600), true);
    description.set_title("UO:ME -- 0 fps");
    mainWindow_.reset(new CL_DisplayWindow(description));

    windowManager_.reset(new CL_GUIWindowManagerTexture(*mainWindow_));

    boost::filesystem::path themeDirPath = config["ui.themes-directory"].as<std::string>();
    boost::filesystem::path path = config["ui.default-theme"].as<std::string>();
    path = themeDirPath / path;
    guiManager_.reset(new CL_GUIManager(*windowManager_, path.string()));

    renderQueue_.reset(new RenderQueue());
}

Manager::~Manager() {
}

bool Manager::shouldExit() {
    return guiManager_->get_exit_flag();
}

void Manager::processMessages() {
    windowManager_->process();
}

void Manager::drawWindow() {
    windowManager_->draw_windows(getGraphicsContext());
    mainWindow_->flip();
}

CL_DisplayWindow* Manager::getMainWindow() {
    return mainWindow_.get();
}

CL_GraphicContext& Manager::getGraphicsContext() {
    return mainWindow_->get_gc();
}

//CL_InputContext& Manager::getIC() {
    //return window_->get_ic();
//}

CL_Texture* Manager::provideTexture(unsigned int width, unsigned int height) {
    return new CL_Texture(getGraphicsContext(), width, height, cl_rgb8);
}

IngameWindow* Manager::getIngameWindow() {
    if (ingameWindow_.get() == NULL) {
        ingameWindow_.reset(new IngameWindow(guiManager_.get()));
        ingameWindow_->set_geometry(CL_Rect(150, 150, 700, 550));
    }

    return ingameWindow_.get();
}

RenderQueue* Manager::getRenderQueue() {
    return renderQueue_.get();
}

}
}
