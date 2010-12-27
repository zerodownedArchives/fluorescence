
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

Manager::Manager(const boost::program_options::variables_map& config) {
    CL_SetupCore clSetupCore_;
    CL_SetupDisplay clSetupDisplay_;
    CL_SetupGL clSetupGL_;

    CL_OpenGLWindowDescription description;
    description.set_size(CL_Size(800, 600), true);
    description.set_title("UO:ME");

    window_ = boost::shared_ptr<CL_DisplayWindow>(new CL_DisplayWindow(description));
    renderer_ = boost::shared_ptr<Renderer>(new Renderer(getGC()));
}

Manager::~Manager() {
}

boost::shared_ptr<CL_DisplayWindow> Manager::getWindow() {
    return window_;
}

CL_GraphicContext& Manager::getGC() {
    return window_->get_gc();
}

CL_InputContext& Manager::getIC() {
    return window_->get_ic();
}

boost::shared_ptr<CL_Texture> Manager::provideTexture(unsigned int width, unsigned int height) {
    return boost::shared_ptr<CL_Texture>(new CL_Texture(getGC(), width, height, cl_rgb8));
}

boost::shared_ptr<Renderer> Manager::getRenderer() {
    return renderer_;
}

}
}
