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



#include "manager.hpp"

#include <boost/filesystem/operations.hpp>

#include "cursormanager.hpp"
#include "gumpmenu.hpp"
#include "xmlparser.hpp"
#include "gumpmenus.hpp"
#include "gumpactions.hpp"
#include "fontengine.hpp"
#include "render/shadermanager.hpp"
#include "uofont.hpp"
#include "cliprectmanager.hpp"
#include "audiomanager.hpp"
#include "commandmanager.hpp"
#include "macromanager.hpp"

#include <client.hpp>

#include <misc/log.hpp>
#include <misc/exception.hpp>

#include <world/ingameobject.hpp>
#include <world/manager.hpp>
#include <world/playerwalkmanager.hpp>

#include <net/manager.hpp>
#include <net/packets/speechrequest.hpp>

namespace fluo {
namespace ui {

Manager* Manager::singleton_ = NULL;

bool Manager::create() {
    if (!singleton_) {
        try {
            singleton_ = new Manager();
        } catch (const std::exception& ex) {
            LOG_EMERGENCY << "Error initializing ui::Manager: " << ex.what() << std::endl;
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
        throw Exception("fluo::ui::Manager Singleton called before being created");
    }

    return singleton_;
}

Manager::Manager() {
    CL_OpenGLWindowDescription description;
    description.set_size(CL_Size(1024, 768), true);
    description.set_title("fluorescence");
    description.set_allow_resize(true);
    description.set_depth_size(24);
    description.set_stencil_size(8);
    description.set_version(2, 1, true);
    mainWindow_.reset(new CL_DisplayWindow(description));

    slotCloseWindow = mainWindow_->sig_window_close().connect(Client::getSingleton(), &Client::shutdown);

    windowManager_.reset(new CL_GUIWindowManagerTexture(*mainWindow_));

    boost::filesystem::path path = "themes";
    path = path / "default";
    guiManager_.reset(new CL_GUIManager(*windowManager_, path.string()));

    path = "fonts";
    loadFontDirectory(path);
    
    LOG_DEBUG << "OpenGL extensions: " << getOpenGLExtensions() << std::endl;
}

bool Manager::setShardConfig(Config& config) {
    boost::filesystem::path path = "shards";
    path = path / config["/fluo/shard@name"].asPath() / "themes" / config["/fluo/ui/theme@name"].asPath();

    if (!boost::filesystem::exists(path)) {
        path = "themes";
        path = path / config["/fluo/ui/theme@name"].asPath();

        if (!boost::filesystem::exists(path)) {
            LOG_EMERGENCY << "Unable to load theme, directory does not exist: " << config["/fluo/ui/theme@name"].asPath() << std::endl;
            return false;
        }
    }

    guiManager_->exit_with_code(0);
    guiManager_.reset(new CL_GUIManager(*windowManager_, path.string()));
    
    guiManager_->func_input_received_nowindow().set(this, &Manager::onInputOutsideWindows);

    path = "fonts";
    loadFontDirectory(path);

    path = "shards";
    path = path / config["/fluo/shard@name"].asPath() / "fonts";
    loadFontDirectory(path);
    
    loadUnifonts();
    
    fontEngine_.reset(new FontEngine(config, getGraphicContext()));

    cursorManager_.reset(new CursorManager(config, mainWindow_));

    shaderManager_.reset(new ShaderManager(getGraphicContext()));
    
    clipRectManager_.reset(new ClipRectManager());
    
    audioManager_.reset(new AudioManager(config));
    
    commandManager_.reset(new CommandManager(config));
    macroManager_.reset(new MacroManager(config));
    
    doubleClickTimeout_ = config["/fluo/input/mouse@doubleclick-timeout-ms"].asInt();
    
    return true;
}

Manager::~Manager() {
    LOG_INFO << "ui::Manager shutdown" << std::endl;
}

void Manager::stepInput(unsigned int elapsedMillis) {
    CL_KeepAlive::process();
    
    // waiting for a doubleclick?
    if (singleClickWait_.first) {
        if (singleClickWait_.second <= elapsedMillis) {
            singleClickWait_.first->onClick();
            singleClickWait_.first = nullptr;
        } else {
            singleClickWait_.second -= elapsedMillis;
        }
    }
    
    processGumpCloseList();
    audioManager_->step();
}

void Manager::stepDraw() {
    windowManager_->process();
    
    getGraphicContext().clear();
    windowManager_->draw_windows(getGraphicContext());
    mainWindow_->flip(); // use parameter 1 here for vsync
}

boost::shared_ptr<CL_DisplayWindow> Manager::getMainWindow() {
    return singleton_->mainWindow_;
}

CL_GraphicContext& Manager::getGraphicContext() {
    return singleton_->mainWindow_->get_gc();
}

CL_InputContext& Manager::getInputContext() {
    return singleton_->mainWindow_->get_ic();
}

CL_Subtexture Manager::provideTexture(unsigned int usage, const CL_Size& size) {
    if (textureGroups_[usage].is_null()) {
        textureGroups_[usage] = CL_TextureGroup(CL_Size(TEXTURE_GROUP_WIDTH, TEXTURE_GROUP_HEIGHT));
    }
    return textureGroups_[usage].add(getGraphicContext(), size);
}

CL_Texture Manager::providerRenderBufferTexture(const CL_Size& size, CL_TextureFormat format) {
    return CL_Texture(getGraphicContext(), size, format);
}

void Manager::freeTexture(unsigned int usage, CL_Subtexture& texture) {
    textureGroups_[usage].remove(texture);
}

boost::shared_ptr<CL_GUIManager> Manager::getGuiManager() {
    return singleton_->guiManager_;
}

boost::shared_ptr<CursorManager> Manager::getCursorManager() {
    return singleton_->cursorManager_;
}

boost::shared_ptr<FontEngine> Manager::getFontEngine() {
    return singleton_->fontEngine_;
}

boost::shared_ptr<AudioManager> Manager::getAudioManager() {
    return singleton_->audioManager_;
}

void Manager::closeGumpMenu(GumpMenu* menu) {
    gumpCloseList_.push_back(menu);
}

void Manager::closeGumpMenu(const UnicodeString& gumpName) {
    GumpMenu* menu = getGumpMenu(gumpName);
    if (menu) {
        closeGumpMenu(menu);
    }
}

void Manager::destroyAllGumpMenus() {
    std::list<GumpMenu*>::iterator iter = gumpList_.begin();
    std::list<GumpMenu*>::iterator end = gumpList_.end();

    for (; iter != end; ++iter) {
        delete *iter;
    }

    gumpList_.clear();
}

GumpMenu* Manager::getGumpMenu(const UnicodeString& gumpName) {
    std::list<GumpMenu*>::iterator iter = gumpList_.begin();
    std::list<GumpMenu*>::iterator end = gumpList_.end();

    for (; iter != end; ++iter) {
        if ((*iter)->getName() == gumpName) {
            return *iter;
        }
    }

    return NULL;
}

void Manager::processGumpCloseList() {
    std::list<GumpMenu*>::iterator iter = gumpCloseList_.begin();
    std::list<GumpMenu*>::iterator end = gumpCloseList_.end();

    for (; iter != end; ++iter) {
        (*iter)->onClose();
        delete (*iter);
        gumpList_.remove(*iter);
    }

    gumpCloseList_.clear();
}

void Manager::loadFontDirectory(const boost::filesystem::path& path) {
    namespace bfs = boost::filesystem;

    if (!bfs::exists(path) || !bfs::is_directory(path)) {
        LOG_ERROR << "Unable to load fonts directory " << path << std::endl;
        return;
    }

    bfs::directory_iterator iter(path);
    bfs::directory_iterator end;

    for (; iter != end; ++iter) {
        if (bfs::is_directory(iter->status())) {
            loadFontDirectory(iter->path());
        } else {
            if (iter->path().extension() != ".ttf") {
                LOG_ERROR << "Unable to load font, only ttf fonts supported: " << iter->path() << std::endl;
                continue;
            }

            LOG_DEBUG << "Registering system font " << iter->path().stem() << " from path " << iter->path().string() << std::endl;
            CL_Font_System::register_font(iter->path().string(), iter->path().stem());
        }
    }
}

GumpMenu* Manager::openXmlGump(const UnicodeString& name) {
    return XmlParser::fromXmlFile(name);
}

void Manager::registerGumpMenu(GumpMenu* menu) {
    gumpList_.push_back(menu);
}

void Manager::installMacros() {
    // TODO: remove accelerator stuff
    CL_AcceleratorKey keyEnter(CL_KEY_B);
    keyEnter.set_shift(true);
    keyEnter.func_pressed().set(this, &Manager::enterTest);
    macros_.add_accelerator(keyEnter);

    guiManager_->set_accelerator_table(macros_);
    
    macroManager_->init();
}

void Manager::uninstallMacros() {
    // TODO: remove accelerator stuff
    CL_AcceleratorTable empty;
    macros_ = empty;

    guiManager_->set_accelerator_table(macros_);
    
    macroManager_->clear();
}

void Manager::enterTest(CL_GUIMessage msg, CL_AcceleratorKey key) {
    // TODO: make this a command
    LOG_DEBUG << "accel Enter" << std::endl;

    GumpMenu* gameWindow = getGumpMenu("gamewindow");
    if (gameWindow) {
        gameWindow->activatePage(2);

        CL_GUIComponent* lineedit = gameWindow->get_named_item("speechtext");
        if (lineedit) {
            lineedit->set_focus();
        } else {
            LOG_ERROR << "Unable to find speech lineedit in gamewindow" << std::endl;
        }
    } else {
        LOG_ERROR << "Unable to find gamewindow gump to activate speech lineedit" << std::endl;
    }
}

void Manager::onSingleClick(boost::shared_ptr<world::IngameObject> obj) {
    if (cursorManager_->hasTarget()) {
        cursorManager_->onTarget(obj);
    } else if (singleClickWait_.first) {
        // we are already waiting for a doubleclick    
        obj->onDoubleClick();
        singleClickWait_.first = nullptr;
    } else {
        // emit single click event in doubleClickTimeout_ milliseconds
        singleClickWait_.first = obj.get();
        singleClickWait_.second = doubleClickTimeout_;
    }
}

void Manager::onDoubleClick(boost::shared_ptr<world::IngameObject> obj) {
    singleClickWait_.first = nullptr;
    
    if (cursorManager_->hasTarget()) {
        cursorManager_->onTarget(obj);
    } else {
        obj->onDoubleClick();
    }
}

void Manager::onDragDrop(boost::shared_ptr<world::IngameObject> dragObj, boost::shared_ptr<world::IngameObject> dragTarget) {
    if (dragTarget) {
        dragObj->onDraggedOnto(dragTarget);
    } else {
        dragObj->onDraggedToVoid();
    }
}

boost::shared_ptr<ShaderManager> Manager::getShaderManager() {
    return getSingleton()->shaderManager_;
}

UnicodeString Manager::getOpenGLExtensions() const {
    const GLubyte* sExtensions = glGetString(GL_EXTENSIONS);
    return UnicodeString((const char*)sExtensions);
}

void Manager::loadUnifonts() {
    unifonts_[0].reset(new UoFont(0));
    CL_FontDescription fontDesc;
    fontDesc.set_typeface_name("unifont");
    LOG_DEBUG << "Registering font unifont" << std::endl;
    getGuiManager()->register_font(*(unifonts_[0].get()), fontDesc);
    
    for (unsigned int i = 1; i <= 12; ++i) {
        unifonts_[i].reset(new UoFont(i));
        CL_FontDescription curDesc;
        std::stringstream sstr;
        sstr << "unifont" << i;
        curDesc.set_typeface_name(sstr.str());
        LOG_DEBUG << "Registering font " << sstr.str() << std::endl;
        getGuiManager()->register_font(*(unifonts_[i].get()), curDesc);
    }
}

boost::shared_ptr<ClipRectManager> Manager::getClipRectManager() {
    return getSingleton()->clipRectManager_;
}

const UoFont& Manager::getUnifont(unsigned int index) {
    if (index < 13) {
        return *(getSingleton()->unifonts_[index].get());
    } else {
        LOG_WARN << "Trying to access unifont with id " << index << std::endl;
        return *(getSingleton()->unifonts_[0].get());
    }
}

boost::shared_ptr<CommandManager> Manager::getCommandManager() {
    return getSingleton()->commandManager_;
}

boost::shared_ptr<MacroManager> Manager::getMacroManager() {
    return getSingleton()->macroManager_;
}

bool Manager::onUnhandledInputEvent(const CL_InputEvent& event) {
    if (event.type == CL_InputEvent::pressed) {
        return macroManager_->execute(event);
    }
    
    return false;
}

void Manager::onInputOutsideWindows(const CL_InputEvent& event, const CL_InputState& state) {
    // LOG_DEBUG << "event outside: " << event.type << std::endl;
    
    if (event.type == CL_InputEvent::released && event.id == CL_MOUSE_RIGHT) {
        world::Manager::getPlayerWalkManager()->stopAtNextTile();
    }
}

}
}
