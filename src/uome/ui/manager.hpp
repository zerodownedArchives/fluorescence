#ifndef UOME_GUI_MANAGER_HPP
#define UOME_GUI_MANAGER_HPP

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#include <boost/shared_ptr.hpp>
#include <boost/program_options.hpp>

namespace uome {
namespace ui {

class IngameWindow;
class RenderQueue;

class Manager {
public:
    static bool create(const boost::program_options::variables_map& config);
    static Manager* getSingleton();

    boost::shared_ptr<CL_DisplayWindow> getWindow();

    CL_GraphicContext& getGC();

    CL_InputContext& getIC();

    boost::shared_ptr<CL_Texture> provideTexture(unsigned int width, unsigned int height);

    boost::shared_ptr<IngameWindow> getIngameWindow();
    boost::shared_ptr<RenderQueue> getRenderQueue();

private:
    static Manager* singleton_;

    Manager(const boost::program_options::variables_map& config);
    ~Manager();
    Manager(const Manager& copy) { }
    void operator=(const Manager& copy) { }

    boost::shared_ptr<CL_DisplayWindow> window_;

    boost::shared_ptr<RenderQueue> renderQueue_;
    boost::shared_ptr<IngameWindow> ingameWindow_;
};

}
}

#endif
