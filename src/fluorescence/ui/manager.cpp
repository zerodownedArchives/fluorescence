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
#include "doubleclickhandler.hpp"
#include "gumpmenu.hpp"
#include "xmlparser.hpp"
#include "gumpmenus.hpp"
#include "gumpactions.hpp"
#include "fontengine.hpp"
#include "render/shadermanager.hpp"
#include "uofont.hpp"
#include "cliprectmanager.hpp"

#include <client.hpp>

#include <misc/log.hpp>
#include <misc/exception.hpp>

#include <world/ingameobject.hpp>

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
    description.set_depth_size(8);
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

    path = "fonts";
    loadFontDirectory(path);

    path = "shards";
    path = path / config["/fluo/shard@name"].asPath() / "fonts";
    loadFontDirectory(path);
    
    loadUnifonts();
    
    fontEngine_.reset(new FontEngine(config, getGraphicContext()));

    cursorManager_.reset(new CursorManager(config, mainWindow_));
    singleton_->getCursorManager()->setCursor(CursorType::GAME_WEST);

    doubleClickHandler_.reset(new DoubleClickHandler(config));
    doubleClickHandler_->start();

    shaderManager_.reset(new ShaderManager(getGraphicContext()));
    
    clipRectManager_.reset(new ClipRectManager());
    
    return true;
}

Manager::~Manager() {
    LOG_INFO << "ui::Manager shutdown" << std::endl;
}

void Manager::stepInput() {
    while (!singleClickQueue_.empty()) {
        boost::shared_ptr<world::IngameObject> obj = singleClickQueue_.front();
        singleClickQueue_.pop();

        if (cursorManager_->hasTarget()) {
            cursorManager_->onTarget(obj);
        } else {
            obj->onClick();
        }
    }

    while (!doubleClickQueue_.empty()) {
        boost::shared_ptr<world::IngameObject> obj = doubleClickQueue_.front();
        doubleClickQueue_.pop();
        //obj->onDoubleClick();
    }

    while (!dragQueue_.empty()) {
        std::pair<boost::shared_ptr<world::IngameObject>, boost::shared_ptr<world::IngameObject> > dragPair = dragQueue_.front();
        dragQueue_.pop();

        if (dragPair.second) {
            dragPair.first->onDraggedOnto(dragPair.second);
        } else {
            dragPair.first->onDraggedToVoid();
        }
    }
}

void Manager::stepDraw() {
    windowManager_->process();
    
    getGraphicContext().clear();
    windowManager_->draw_windows(getGraphicContext());
    mainWindow_->flip(); // use parameter 1 here for vsync

    CL_KeepAlive::process();

    processGumpCloseList();
}

boost::shared_ptr<CL_DisplayWindow> Manager::getMainWindow() {
    return singleton_->mainWindow_;
}

CL_GraphicContext& Manager::getGraphicContext() {
    return singleton_->mainWindow_->get_gc();
}

CL_Texture* Manager::provideTexture(unsigned int width, unsigned int height) {
    return new CL_Texture(singleton_->getGraphicContext(), width, height, cl_rgb8);
}

boost::shared_ptr<CL_GUIManager> Manager::getGuiManager() {
    return singleton_->guiManager_;
}

boost::shared_ptr<CursorManager> Manager::getCursorManager() {
    return singleton_->cursorManager_;
}

boost::shared_ptr<DoubleClickHandler> Manager::getDoubleClickHandler() {
    return singleton_->doubleClickHandler_;
}

boost::shared_ptr<FontEngine> Manager::getFontEngine() {
    return singleton_->fontEngine_;
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
    CL_AcceleratorKey keyEnter(CL_KEY_ENTER);
    keyEnter.func_pressed().set(this, &Manager::enterTest);
    macros_.add_accelerator(keyEnter);

    guiManager_->set_accelerator_table(macros_);
}

void Manager::uninstallMacros() {
    CL_AcceleratorTable empty;
    macros_ = empty;

    guiManager_->set_accelerator_table(macros_);
}

void Manager::enterTest(CL_GUIMessage msg, CL_AcceleratorKey key) {
    LOG_DEBUG << "accel Enter pressed" << std::endl;

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

void Manager::queueSingleClick(boost::shared_ptr<world::IngameObject> obj) {
    singleClickQueue_.push(obj);
}

void Manager::queueDoubleClick(boost::shared_ptr<world::IngameObject> obj) {
    doubleClickQueue_.push(obj);
}


void Manager::queueDrag(boost::shared_ptr<world::IngameObject> dragObj, boost::shared_ptr<world::IngameObject> dragTarget) {
    dragQueue_.push(std::make_pair(dragObj, dragTarget));
}

void Manager::onClickEvent(boost::shared_ptr<world::IngameObject> obj) {
    if (cursorManager_->hasTarget()) {
        queueSingleClick(obj);
    } else {
        doubleClickHandler_->notify(obj);
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
    unifonts_[0] = UoFont(0);
    CL_FontDescription fontDesc;
    fontDesc.set_typeface_name("unifont");
    LOG_DEBUG << "Registering font unifont" << std::endl;
    getGuiManager()->register_font(unifonts_[0], fontDesc);
    
    for (unsigned int i = 1; i <= 12; ++i) {
        unifonts_[i] = UoFont(i);
        CL_FontDescription curDesc;
        std::stringstream sstr;
        sstr << "unifont" << i;
        curDesc.set_typeface_name(sstr.str());
        LOG_DEBUG << "Registering font " << sstr.str() << std::endl;
        getGuiManager()->register_font(unifonts_[i], curDesc);
    }
}

boost::shared_ptr<ClipRectManager> Manager::getClipRectManager() {
    return getSingleton()->clipRectManager_;
}

const UoFont& Manager::getUnifont(unsigned int index) {
    if (index < 13) {
        return getSingleton()->unifonts_[index];
    } else {
        LOG_WARN << "Trying to access unifont with id " << index << std::endl;
        return getSingleton()->unifonts_[0];
    }
}

}
}
