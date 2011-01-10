
#include "manager.hpp"

#include <boost/filesystem/path.hpp>

#include "ingameview.hpp"
#include "renderqueue.hpp"

#include <misc/logger.hpp>
#include <misc/exception.hpp>

#include <world/manager.hpp>
#include <world/sectormanager.hpp>

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
    renderQueue_->clear();
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

boost::shared_ptr<CL_DisplayWindow> Manager::getMainWindow() {
    return mainWindow_;
}

CL_GraphicContext& Manager::getGraphicsContext() {
    return mainWindow_->get_gc();
}

CL_Texture* Manager::provideTexture(unsigned int width, unsigned int height) {
    return new CL_Texture(getGraphicsContext(), width, height, cl_rgb8);
}

boost::shared_ptr<RenderQueue> Manager::getRenderQueue() {
    return renderQueue_;
}

boost::shared_ptr<CL_GUIManager> Manager::getGuiManager() {
    return guiManager_;
}

}
}
