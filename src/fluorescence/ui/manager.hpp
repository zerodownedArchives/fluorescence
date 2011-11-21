#ifndef FLUO_UI_MANAGER_HPP
#define FLUO_UI_MANAGER_HPP

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/gui.h>

#include <boost/shared_ptr.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include <list>
#include <queue>

#include <misc/config.hpp>

namespace fluo {
namespace world {
    class IngameObject;
}

namespace ui {

class IngameView;
class RenderQueue;
class CursorManager;
class DoubleClickHandler;
class GumpMenu;
class FontEngine;
class ShaderManager;

class Manager {
public:
    static bool create();
    static void destroy();
    static Manager* getSingleton();

    bool setShardConfig(Config& config);

    static CL_GraphicContext& getGraphicContext();
    static boost::shared_ptr<CL_DisplayWindow> getMainWindow();

    static CL_Texture* provideTexture(unsigned int width, unsigned int height);

    static boost::shared_ptr<RenderQueue> getWorldRenderQueue();
    static boost::shared_ptr<CL_GUIManager> getGuiManager();
    static boost::shared_ptr<CursorManager> getCursorManager();
    static boost::shared_ptr<DoubleClickHandler> getDoubleClickHandler();
    static boost::shared_ptr<FontEngine> getFontEngine();
    static boost::shared_ptr<ShaderManager> getShaderManager();

    void step();

    GumpMenu* openXmlGump(const UnicodeString& name);
    void closeGumpMenu(const UnicodeString& name);
    void closeGumpMenu(GumpMenu* menu);

    GumpMenu* getGumpMenu(const UnicodeString& name);

    void registerGumpMenu(GumpMenu* menu);

    void installMacros();
    void uninstallMacros();

    void systemMessage(const UnicodeString& msg);

    // called by the doubleclick handler, resp. the cursor manager
    void queueSingleClick(boost::shared_ptr<world::IngameObject> obj);
    void queueDoubleClick(boost::shared_ptr<world::IngameObject> obj);
    void queueDrag(boost::shared_ptr<world::IngameObject> dragObj, boost::shared_ptr<world::IngameObject> dragTarget);


    // called by the various ui components
    void onClickEvent(boost::shared_ptr<world::IngameObject> obj);

private:
    static Manager* singleton_;

    Manager();
    ~Manager();
    Manager(const Manager& copy) { }
    void operator=(const Manager& copy) { }

    boost::shared_ptr<RenderQueue> worldRenderQueue_;

    // clan lib setup stuff
    CL_SetupCore clSetupCore;
    CL_SetupDisplay clSetupDisplay;
    CL_SetupGL clSetupGL;
    CL_SetupGUI clSetupGUI;

    // clan lib gui stuff
    boost::shared_ptr<CL_GUIManager> guiManager_;
    boost::shared_ptr<CL_GUIWindowManagerTexture> windowManager_;
    boost::shared_ptr<CL_DisplayWindow> mainWindow_;

    boost::shared_ptr<CursorManager> cursorManager_;

    boost::shared_ptr<DoubleClickHandler> doubleClickHandler_;

    boost::recursive_mutex closeListMutex_;
    std::list<GumpMenu*> closeList_;
    void processCloseList();

    CL_Slot slotCloseWindow;

    void loadFontDirectory(const boost::filesystem::path& path);

    boost::recursive_mutex gumpListMutex_;
    std::list<GumpMenu*> gumpList_;

    CL_AcceleratorTable macros_;

    void enterTest(CL_GUIMessage msg, CL_AcceleratorKey key);

    std::queue<boost::shared_ptr<world::IngameObject> > singleClickQueue_;
    std::queue<boost::shared_ptr<world::IngameObject> > doubleClickQueue_;
    std::queue<std::pair<boost::shared_ptr<world::IngameObject>, boost::shared_ptr<world::IngameObject> > > dragQueue_;

    boost::shared_ptr<FontEngine> fontEngine_;

    boost::shared_ptr<ShaderManager> shaderManager_;
};

}
}

#endif
