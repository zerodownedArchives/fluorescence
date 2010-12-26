
#include "manager.hpp"

#include <logger.hpp>
#include <exception.hpp>

namespace uome {
namespace ui {

Manager* Manager::singleton_ = NULL;

bool Manager::create(const boost::program_options::variables_map& config) {
    if (!singleton_) {
        try {
            singleton_ = new Manager(config);
        } catch (const std::exception& ex) {
            LOGARG_CRITICAL(LOGTYPE_UI, "Error initializing data::Manager: %s", ex.what());
            return false;
        }
    }

    return true;
}

Manager* Manager::getSingleton() {
    if (!singleton_) {
        throw Exception("uome::ui::Manager called before being created");
    }

    return singleton_;
}

Manager::Manager(const boost::program_options::variables_map& config) :
        window_(NULL) {
}

Manager::~Manager() {
    if (window_) {
        delete window_;
        window_ = NULL;
    }
}

CL_DisplayWindow* Manager::getWindow() {
    if (!window_) {
        CL_OpenGLWindowDescription description;
        description.set_size(CL_Size(800, 600), true);
        description.set_title("UO:ME");

        window_ = new CL_DisplayWindow(description);
    }

    return window_;
}

CL_GraphicContext& Manager::getGC() {
    if (window_) {
        return window_->get_gc();
    }

    throw Exception("Trying to retrieve GraphicContext before window is initialized");
}

CL_InputContext& Manager::getIC() {
    if (window_) {
        return window_->get_ic();
    }

    throw Exception("Trying to retrieve InputContext before window is initialized");
}

boost::shared_ptr<CL_Texture> Manager::provideTexture(unsigned int width, unsigned int height) {
    return boost::shared_ptr<CL_Texture>(new CL_Texture(getGC(), width, height, cl_rgb8));
}

RenderQueue* Manager::getRenderQueue() {
    return &renderQueue_;
}

}
}
