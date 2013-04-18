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
#include "gumpmenus.hpp"
#include "fontengine.hpp"
#include "render/shadermanager.hpp"
#include "uofont.hpp"
#include "cliprectmanager.hpp"
#include "audiomanager.hpp"
#include "commandmanager.hpp"
#include "macromanager.hpp"
#include "python/scriptloader.hpp"

#include "components/lineedit.hpp"

#include <client.hpp>

#include <data/manager.hpp>

#include <misc/log.hpp>
#include <misc/exception.hpp>

#include <world/ingameobject.hpp>
#include <world/manager.hpp>
#include <world/playerwalkmanager.hpp>

#include <net/manager.hpp>
#include <net/packets/ad_speechrequest.hpp>
#include <net/packets/c2_unicodepromptreply.hpp>

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

Manager::Manager() : worldView_(nullptr), promptSerial_(0) {
    CL_OpenGLWindowDescription description;
    description.set_position(CL_Rect(50, 50, CL_Size(1024, 768)), true);
    description.set_title("fluorescence");
    description.set_allow_resize(true);
    description.set_depth_size(24);
    description.set_stencil_size(8);
    description.set_version(2, 1, true);
    mainWindow_.reset(new CL_DisplayWindow(description));

    LOG_INFO << "OpenGL vendor: " << getOpenGLVendor() << std::endl;
    LOG_INFO << "OpenGL renderer: " << getOpenGLRenderer() << std::endl;

    UnicodeString openglExt = getOpenGLExtensions();
    LOG_INFO << "OpenGL extensions: " << openglExt << std::endl;
    if (openglExt.indexOf("GL_EXT_gpu_shader4") == -1) {
        LOG_EMERGENCY << "Sorry, your graphics card or graphics card driver is not supported. Please try to update the most recent, official driver" << std::endl;
        throw Exception("Graphics card or driver not supported");
    }

    slotCloseWindow = mainWindow_->sig_window_close().connect(Client::getSingleton(), &Client::shutdown);

    windowManager_.reset(new CL_GUIWindowManagerTexture(*mainWindow_));

    guiManager_.reset(new CL_GUIManager(*mainWindow_));
    guiManager_->set_window_manager(*windowManager_);
    guiManager_->func_input_received_nowindow().set(this, &Manager::onInputOutsideWindows);

    boost::filesystem::path path = "fonts";
    loadFontDirectory(path);

    setTheme("preshard");

    LOG_DEBUG << "Initializing Python" << std::endl;
    pythonLoader_.reset(new python::ScriptLoader());
    pythonLoader_->init();
    pythonLoader_->setThemePath(getThemePath());
}

bool Manager::setShardConfig(Config& config) {
    destroyAllGumpMenus();

    boost::filesystem::path path = "fonts";
    loadFontDirectory(path);

    path = config.getShardPath() / "fonts";
    loadFontDirectory(path);

    loadUnifonts();

    LOG_DEBUG << "Initializing font renderer" << std::endl;
    fontEngine_.reset(new FontEngine(config, getGraphicContext()));

    LOG_DEBUG << "Initializing cursor manager" << std::endl;
    cursorManager_.reset(new CursorManager(config, mainWindow_));

    LOG_DEBUG << "Initializing shaders" << std::endl;
    shaderManager_.reset(new ShaderManager());
    if (!shaderManager_->initShaders(getGraphicContext())) {
        return false;
    }

    clipRectManager_.reset(new ClipRectManager());

    audioManager_.reset(new AudioManager(config));

    commandManager_.reset(new CommandManager(config));
    macroManager_.reset(new MacroManager(config));

    doubleClickTimeout_ = config["/fluo/input/mouse@doubleclick-timeout-ms"].asInt();
    clilocPropertiesTimeout_ = config["/fluo/input/mouse@object-properties-timeout-ms"].asInt();

    config["/fluo/specialids/ignore@mapart"].toIntList(mapIgnoreIds_);
    config["/fluo/specialids/ignore@staticart"].toIntList(staticIgnoreIds_);
    config["/fluo/specialids/water@mapart"].toIntList(mapWaterIds_);
    config["/fluo/specialids/water@staticart"].toIntList(staticWaterIds_);
    std::sort(mapIgnoreIds_.begin(), mapIgnoreIds_.end());
    std::sort(staticIgnoreIds_.begin(), staticIgnoreIds_.end());
    std::sort(mapWaterIds_.begin(), mapWaterIds_.end());
    std::sort(staticWaterIds_.begin(), staticWaterIds_.end());

    setTheme(config["/fluo/ui/theme@name"].asString());

    pythonLoader_->setShardConfig(config);

    return true;
}

Manager::~Manager() {
    LOG_INFO << "ui::Manager shutdown" << std::endl;
}

void Manager::stepInput(unsigned int elapsedMillis) {
    CL_KeepAlive::process();

    processGumpNewList();

    if (pythonLoader_->step(elapsedMillis)) {
        // if the gumps were reloaded process the new list again
        processGumpNewList();
    }

    // waiting for a doubleclick?
    if (boost::shared_ptr<world::IngameObject> singleClickWaitObj = singleClickWait_.first.lock()) {
        if (singleClickWait_.second <= elapsedMillis) {
            singleClickWaitObj->onClick();
            singleClickWait_.first.reset();
        } else {
            singleClickWait_.second -= elapsedMillis;
        }
    }

    if (boost::shared_ptr<world::IngameObject> clilocPropObj = mouseOverObject_.lock()) {
        if (clilocPropertiesTimer_ <= elapsedMillis && clilocPropertiesTimer_ != 0) {
            // show cliloc properties
            clilocPropObj->openPropertyListGump(getMousePosition());
            clilocPropertiesTimer_ = 0;
        } else {
            clilocPropertiesTimer_ -= elapsedMillis;
        }
    }

    processGumpCloseList();
}

void Manager::stepAudio() {
    audioManager_->step();
}

void Manager::stepDraw() {
    windowManager_->process();

    getGraphicContext().clear();
    windowManager_->draw_windows(getGraphicContext());

    if (cursorManager_) {
        cursorManager_->drawDragObject(getGraphicContext(), getInputContext().get_mouse().get_position());
    }

    mainWindow_->flip(); // use parameter 1 here for vsync

    if (!componentResizeQueue_.empty()) {
        std::vector<std::pair<CL_GUIComponent*, CL_Rectf> >::iterator iter = componentResizeQueue_.begin();
        std::vector<std::pair<CL_GUIComponent*, CL_Rectf> >::iterator end = componentResizeQueue_.end();
        for (; iter != end; ++iter) {
            iter->first->set_geometry(iter->second);
        }
        componentResizeQueue_.clear();
    }

    if (!componentRepaintQueue_.empty()) {
        std::vector<CL_GUIComponent*>::iterator iter = componentRepaintQueue_.begin();
        std::vector<CL_GUIComponent*>::iterator end = componentRepaintQueue_.end();
        for (; iter != end; ++iter) {
            (*iter)->request_repaint();
        }
        componentRepaintQueue_.clear();
    }
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
    std::list<GumpMenu*>::iterator iter = gumpList_.begin();
    std::list<GumpMenu*>::iterator end = gumpList_.end();

    for (; iter != end; ++iter) {
        if ((*iter)->getName() == gumpName) {
            closeGumpMenu(*iter);
        }
    }
}

void Manager::closeGumpMenuByTypeId(unsigned int typeId) {
    if (typeId == 0) {
        return;
    }

    std::list<GumpMenu*>::iterator iter = gumpList_.begin();
    std::list<GumpMenu*>::iterator end = gumpList_.end();

    for (; iter != end; ++iter) {
        if ((*iter)->getTypeId() == typeId) {
            closeGumpMenu(*iter);
        }
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

    iter = gumpNewList_.begin();
    end = gumpNewList_.end();
    for (; iter != end; ++iter) {
        if ((*iter)->getName() == gumpName) {
            return *iter;
        }
    }

    return nullptr;
}

void Manager::processGumpCloseList() {
    if (gumpCloseList_.size() > 1) {
        gumpCloseList_.unique();
    }

    std::list<GumpMenu*>::iterator iter = gumpCloseList_.begin();
    std::list<GumpMenu*>::iterator end = gumpCloseList_.end();

    for (; iter != end; ++iter) {
        (*iter)->onClose();
        delete (*iter);
        gumpList_.remove(*iter);
    }

    gumpCloseList_.clear();
}

void Manager::processGumpNewList() {
    std::list<GumpMenu*>::iterator iter = gumpNewList_.begin();
    std::list<GumpMenu*>::iterator end = gumpNewList_.end();

    for (; iter != end; ++iter) {
        (*iter)->updateMobileProperties();
        (*iter)->fitSizeToChildren();
        (*iter)->activateFirstPage();
        gumpList_.push_back(*iter);
    }

    gumpNewList_.clear();
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
        if (bfs::is_directory(iter->status()) && iter->path().leaf() != ".svn") {
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

void Manager::openPythonGump(const UnicodeString& name) {
    pythonLoader_->openGump(name);
}

void Manager::openPythonGump(const UnicodeString& name, boost::python::dict& args) {
    pythonLoader_->openGump(name, args);
}

void Manager::registerGumpMenu(GumpMenu* menu) {
    gumpNewList_.push_back(menu);
}

void Manager::installMacros() {
    macroManager_->init();
}

void Manager::uninstallMacros() {
    macroManager_->clear();
}

void Manager::onSingleClick(boost::shared_ptr<world::IngameObject> obj) {
    LOG_DEBUG << "single click" << std::endl;
    if (cursorManager_->hasTarget()) {
        cursorManager_->onTarget(obj);
    } else if (!singleClickWait_.first.expired()) {
        // we are already waiting for a doubleclick
        obj->onDoubleClick();
        singleClickWait_.first.reset();
    } else {
        // emit single click event in doubleClickTimeout_ milliseconds
        singleClickWait_.first = obj;
        singleClickWait_.second = doubleClickTimeout_;
    }
}

void Manager::onDoubleClick(boost::shared_ptr<world::IngameObject> obj) {
    LOG_DEBUG << "double click" << std::endl;
    singleClickWait_.first.reset();

    if (cursorManager_->hasTarget()) {
        cursorManager_->onTarget(obj);
    } else {
        obj->onDoubleClick();
    }
}

void Manager::onDragDrop(boost::shared_ptr<world::IngameObject> dragObj, boost::shared_ptr<world::IngameObject> dragTarget, int locX, int locY) {
    if (dragTarget) {
        dragObj->onDraggedOnto(dragTarget, locX, locY);
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

UnicodeString Manager::getOpenGLVendor() const {
    const GLubyte* sExtensions = glGetString(GL_VENDOR);
    return UnicodeString((const char*)sExtensions);
}

UnicodeString Manager::getOpenGLRenderer() const {
    const GLubyte* sExtensions = glGetString(GL_RENDERER);
    return UnicodeString((const char*)sExtensions);
}

void Manager::loadUnifonts() {
    UoFont uni0(0, false);
    UoFont uni0Border(0, true);
    CL_FontDescription fontDesc;
    fontDesc.set_typeface_name("unifont");
    fontDesc.set_subpixel(false);
    fontDesc.set_weight(400);
    LOG_DEBUG << "Registering font unifont" << std::endl;
    getGuiManager()->register_font(uni0, fontDesc);
    CL_FontDescription fontDescBorder;
    fontDescBorder.clone(fontDesc);
    fontDesc.set_weight(700);
    getGuiManager()->register_font(uni0Border, fontDescBorder);

    for (unsigned int i = 1; i <= 12; ++i) {
        UoFont uniCur(i, false);
        UoFont uniCurBorder(i, true);
        CL_FontDescription curDesc;
        std::stringstream sstr;
        sstr << "unifont" << i;
        curDesc.set_typeface_name(sstr.str());
        curDesc.set_subpixel(false);
        curDesc.set_weight(400);
        CL_FontDescription curDescBorder;
        curDescBorder.clone(curDesc);
        curDescBorder.set_weight(700);

        // check if this unifont file exists
        sstr << ".mul";
        if (data::Manager::hasPathFor(sstr.str())) {
            LOG_DEBUG << "Registering font " << sstr.str() << std::endl;
            getGuiManager()->register_font(uniCur, curDesc);
            getGuiManager()->register_font(uniCurBorder, curDescBorder);

        }
    }
}

boost::shared_ptr<ClipRectManager> Manager::getClipRectManager() {
    return getSingleton()->clipRectManager_;
}

boost::shared_ptr<CommandManager> Manager::getCommandManager() {
    return getSingleton()->commandManager_;
}

boost::shared_ptr<MacroManager> Manager::getMacroManager() {
    return getSingleton()->macroManager_;
}

bool Manager::onUnhandledInputEvent(const CL_InputEvent& event) {
    if (event.type == CL_InputEvent::pressed && macroManager_) {
        return macroManager_->execute(event);
    }

    return false;
}

void Manager::onInputOutsideWindows(const CL_InputEvent& event, const CL_InputState& state) {
     //LOG_DEBUG << "event outside: " << event.type << std::endl;

    if (event.type == CL_InputEvent::released && event.id == CL_MOUSE_RIGHT) {
        world::Manager::getPlayerWalkManager()->stopAtNextTile();
    }
}

bool Manager::isMapIdIgnored(unsigned int id) {
    return std::binary_search(singleton_->mapIgnoreIds_.begin(), singleton_->mapIgnoreIds_.end(), id);
}

bool Manager::isStaticIdIgnored(unsigned int id) {
    return std::binary_search(singleton_->staticIgnoreIds_.begin(), singleton_->staticIgnoreIds_.end(), id);
}

bool Manager::isMapIdWater(unsigned int id) {
    return std::binary_search(singleton_->mapWaterIds_.begin(), singleton_->mapWaterIds_.end(), id);
}

bool Manager::isStaticIdWater(unsigned int id) {
    return std::binary_search(singleton_->staticWaterIds_.begin(), singleton_->staticWaterIds_.end(), id);
}

void Manager::queueComponentResize(CL_GUIComponent* comp, const CL_Rectf& geom) {
    componentResizeQueue_.push_back(std::make_pair(comp, geom));
}

void Manager::queueComponentRepaint(CL_GUIComponent* comp) {
    componentRepaintQueue_.push_back(comp);
}

void Manager::releaseIngameObjects() {
    singleClickWait_.first.reset();
    mouseOverObject_.reset();
    if (cursorManager_) {
        cursorManager_->releaseIngameObjects();
    }
}

void Manager::closeAllNonMessageGumps() {
    std::list<GumpMenu*>::iterator iter = gumpList_.begin();
    std::list<GumpMenu*>::iterator end = gumpList_.end();
    std::list<GumpMenu*> newList;

    for (; iter != end; ++iter) {
        if ((*iter)->getName() != "messagebox") {
            delete *iter;
        } else {
            newList.push_back(*iter);
        }
    }

    gumpList_.clear();
    gumpList_ = newList;
}

CL_Point Manager::getMousePosition() const {
    CL_InputDevice& mouse = getInputContext().get_mouse();
    return CL_Point(mouse.get_x(), mouse.get_y());
}

void Manager::setWorldView(components::WorldView* view) {
    worldView_ = view;
}

components::WorldView* Manager::getWorldView() const {
    return worldView_;
}

void Manager::setTheme(const UnicodeString& themeName) {
    LOG_DEBUG << "Setting theme: " << themeName << std::endl;
    boost::filesystem::path path("themes");
    path = path / StringConverter::toUtf8String(themeName);

    themePath_ = data::Manager::getShardFilePath(path);

    if (!boost::filesystem::exists(themePath_)) {
        LOG_ERROR << "Theme " << themeName << " was not found" << std::endl;
        throw Exception("Unable to load theme");
    }
}

const boost::filesystem::path& Manager::getThemePath() const {
    return themePath_;
}

CL_Font Manager::getFont(const CL_FontDescription& desc) {
    // check if this is a special UO font (unifont)
    CL_Font ret = getSingleton()->guiManager_->get_registered_font(desc);
    if (ret.is_null()) {
        // return system font if unknown
        ret = CL_Font_System(getSingleton()->getGraphicContext(), desc);
    }
    return ret;
}

void Manager::setPrompt(Serial serial) {
    promptSerial_ = serial;
    showSpeechEntry(true);
}

bool Manager::hasPrompt() const {
    return promptSerial_ != 0;
}

void Manager::handlePrompt(const UnicodeString& text) {
    if (promptSerial_) {
        net::packets::UnicodePromptReply pkt(promptSerial_, text);
        net::Manager::getSingleton()->send(pkt);
        promptSerial_ = 0;
    }
}

void Manager::cancelPrompt() {
    net::packets::UnicodePromptReply pkt(promptSerial_, "");
    net::Manager::getSingleton()->send(pkt);
    promptSerial_ = 0;
}

void Manager::showSpeechEntry(bool clearText) {
    GumpMenu* gameWindow = getGumpMenu("gamewindow");
    if (gameWindow) {
        gameWindow->activatePage(2);
        CL_GUIComponent* lineedit = gameWindow->get_named_item("speechtext");
        if (lineedit) {
            gameWindow->set_visible(); // activate this window
            lineedit->set_focus();
            if (clearText) {
                components::LineEdit* le = dynamic_cast<components::LineEdit*>(lineedit);
                if (le) {
                    le->setText("");
                }
            }
        } else {
            LOG_ERROR << "Unable to find speech lineedit in gamewindow" << std::endl;
        }
    } else {
        LOG_ERROR << "Unable to find gamewindow gump to activate speech lineedit" << std::endl;
    }
}

boost::shared_ptr<python::ScriptLoader> Manager::getPythonLoader() {
    return getSingleton()->pythonLoader_;
}

void Manager::handleTextInput(const UnicodeString& text) {
    if (text.length() <= 0) {
        return;
    }

    // if a prompt is currently active, send response
    if (hasPrompt()) {
        handlePrompt(text);
    } else {
        commandManager_->handleTextInput(text);
    }
}

std::list<GumpMenu*>::iterator Manager::gumpsBegin() {
    return gumpList_.begin();
}

std::list<GumpMenu*>::iterator Manager::gumpsEnd() {
    return gumpList_.end();
}

void Manager::saveDesktop() {
    boost::filesystem::path desktopPath = Client::getSingleton()->getConfig().getShardPath() / "desktop.py";
    pythonLoader_->saveToFile(desktopPath);
}

void Manager::restoreDesktop() {
    boost::filesystem::path desktopPath = Client::getSingleton()->getConfig().getShardPath() / "desktop.py";
    LOG_DEBUG << "Restoring desktop from " << desktopPath.string() << std::endl;
    pythonLoader_->loadFromFile(desktopPath);

    // make sure that there is at least the game window if loading has failed
    if (!getGumpMenu("gamewindow")) {
        LOG_INFO << "No gamewindow found after restoring desktop, opening default" << std::endl;
        openPythonGump("gamewindow");
    }
}

void Manager::setMouseOverObject(const boost::shared_ptr<world::IngameObject>& obj) {
    boost::shared_ptr<world::IngameObject> curObj = mouseOverObject_.lock();
    if (curObj == obj) {
        return;
    }

    if (curObj) {
        curObj->setMouseOver(false);
        closeGumpMenu("objectpropertylist");
    }

    if (obj) {
        obj->setMouseOver(true);
        clilocPropertiesTimer_ = clilocPropertiesTimeout_;
    }

    mouseOverObject_ = obj;
}

}
}
