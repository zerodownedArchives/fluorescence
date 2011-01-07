#ifndef UOME_GUI_MANAGER_HPP
#define UOME_GUI_MANAGER_HPP

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/gui.h>

#include <boost/shared_ptr.hpp>
#include <boost/program_options.hpp>

namespace uome {
namespace ui {

class IngameWindow;
class RenderQueue;

class Manager {
public:
    static bool create(const boost::program_options::variables_map& config);
    static void destroy();
    static Manager* getSingleton();

    bool shouldExit();

    void processMessages();
    void drawWindow();

    CL_GraphicContext& getGraphicsContext();
    CL_DisplayWindow* getMainWindow();

    CL_Texture* provideTexture(unsigned int width, unsigned int height);

    RenderQueue* getRenderQueue();


    IngameWindow* getIngameWindow();

private:
    static Manager* singleton_;

    Manager(const boost::program_options::variables_map& config);
    ~Manager();
    Manager(const Manager& copy) { }
    void operator=(const Manager& copy) { }

    boost::shared_ptr<RenderQueue> renderQueue_;

    // clan lib setup stuff
    CL_SetupCore clSetupCore;
    CL_SetupDisplay clSetupDisplay;
    CL_SetupGL clSetupGL;
    CL_SetupGUI clSetupGUI;

    // clan lib gui stuff
    boost::shared_ptr<CL_GUIManager> guiManager_;
    boost::shared_ptr<CL_GUIWindowManagerTexture> windowManager_;
    boost::shared_ptr<CL_DisplayWindow> mainWindow_;


    boost::shared_ptr<IngameWindow> ingameWindow_;
};

}
}

#endif
