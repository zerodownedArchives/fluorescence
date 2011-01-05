
#include "manager.hpp"

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
    CL_SetupCore clSetupCore_;
    CL_SetupDisplay clSetupDisplay_;
    CL_SetupGL clSetupGL_;

    CL_OpenGLWindowDescription description;
    description.set_size(CL_Size(800, 600), true);
    description.set_title("UO:ME -- 0 fps");

    window_.reset(new CL_DisplayWindow(description));

    renderQueue_.reset(new RenderQueue());
}

Manager::~Manager() {
}

CL_DisplayWindow* Manager::getWindow() {
    return window_.get();
}

CL_GraphicContext& Manager::getGC() {
    return window_->get_gc();
}

CL_InputContext& Manager::getIC() {
    return window_->get_ic();
}

CL_Texture* Manager::provideTexture(unsigned int width, unsigned int height) {
    return new CL_Texture(getGC(), width, height, cl_rgb8);
}

IngameWindow* Manager::getIngameWindow() {
    if (ingameWindow_.get() == NULL) {
        ingameWindow_.reset(new IngameWindow(800, 600));
    }

    return ingameWindow_.get();
}

RenderQueue* Manager::getRenderQueue() {
    return renderQueue_.get();
}

}
}
