/*
 * fluorescence is a free, customizable Ultima Online client.
 * Copyright (C) 2011-2012, http://fluorescence-client.org

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


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

#include <list>
#include <queue>
#include <map>

#include <misc/config.hpp>

namespace fluo {
namespace world {
    class IngameObject;
}

namespace ui {

class IngameView;
class CursorManager;
class DoubleClickHandler;
class GumpMenu;
class FontEngine;
class ShaderManager;
class ClipRectManager;
class UoFont;
class AudioManager;
class CommandManager;
class MacroManager;

class Manager {
public:
    static bool create();
    static void destroy();
    static Manager* getSingleton();
    
    static const unsigned int TEXTURE_GROUP_WIDTH = 2048;
    static const unsigned int TEXTURE_GROUP_HEIGHT = 2048;

    bool setShardConfig(Config& config);

    static CL_GraphicContext& getGraphicContext();
    static CL_InputContext& getInputContext();
    static boost::shared_ptr<CL_DisplayWindow> getMainWindow();

    CL_Subtexture provideTexture(unsigned int usage, const CL_Size& size);
    void freeTexture(unsigned int usage, CL_Subtexture& texture);
    CL_Texture providerRenderBufferTexture(const CL_Size& size, CL_TextureFormat format = cl_rgba);

    static boost::shared_ptr<CL_GUIManager> getGuiManager();
    static boost::shared_ptr<CursorManager> getCursorManager();
    static boost::shared_ptr<DoubleClickHandler> getDoubleClickHandler();
    static boost::shared_ptr<FontEngine> getFontEngine();
    static boost::shared_ptr<ShaderManager> getShaderManager();
    static boost::shared_ptr<ClipRectManager> getClipRectManager();
    static boost::shared_ptr<AudioManager> getAudioManager();
    static boost::shared_ptr<CommandManager> getCommandManager();
    static boost::shared_ptr<MacroManager> getMacroManager();
    
    static const UoFont& getUnifont(unsigned int index);

    void stepInput();
    void stepDraw();

    GumpMenu* openXmlGump(const UnicodeString& name);
    void closeGumpMenu(const UnicodeString& name);
    void closeGumpMenu(GumpMenu* menu);
    void destroyAllGumpMenus(); // needed for final cleanup

    GumpMenu* getGumpMenu(const UnicodeString& name);

    void registerGumpMenu(GumpMenu* menu);

    void installMacros();
    void uninstallMacros();

    // called by the doubleclick handler, resp. the cursor manager
    void queueSingleClick(boost::shared_ptr<world::IngameObject> obj);
    void queueDoubleClick(boost::shared_ptr<world::IngameObject> obj);
    void queueDrag(boost::shared_ptr<world::IngameObject> dragObj, boost::shared_ptr<world::IngameObject> dragTarget);

    // called by the various ui components
    void onClickEvent(boost::shared_ptr<world::IngameObject> obj);
    
    bool onUnhandledInputEvent(const CL_InputEvent& event);
    
private:
    static Manager* singleton_;

    Manager();
    ~Manager();
    Manager(const Manager& copy) { }
    void operator=(const Manager& copy) { }

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

    std::list<GumpMenu*> gumpList_;
    std::list<GumpMenu*> gumpCloseList_;
    void processGumpCloseList();

    CL_Slot slotCloseWindow;

    void loadFontDirectory(const boost::filesystem::path& path);
    void loadUnifonts();
    boost::shared_ptr<UoFont> unifonts_[13];

    CL_AcceleratorTable macros_;

    void enterTest(CL_GUIMessage msg, CL_AcceleratorKey key);

    std::queue<boost::shared_ptr<world::IngameObject> > singleClickQueue_;
    std::queue<boost::shared_ptr<world::IngameObject> > doubleClickQueue_;
    std::queue<std::pair<boost::shared_ptr<world::IngameObject>, boost::shared_ptr<world::IngameObject> > > dragQueue_;

    boost::shared_ptr<FontEngine> fontEngine_;

    boost::shared_ptr<ShaderManager> shaderManager_;
    
    UnicodeString getOpenGLExtensions() const;
    
    boost::shared_ptr<ClipRectManager> clipRectManager_;
    
    boost::shared_ptr<AudioManager> audioManager_;
    
    boost::shared_ptr<CommandManager> commandManager_;
    boost::shared_ptr<MacroManager> macroManager_;
    
    std::map<unsigned int, CL_TextureGroup> textureGroups_;
    
    void onInputOutsideWindows(const CL_InputEvent& event, const CL_InputState& state);
};

}
}

#endif
