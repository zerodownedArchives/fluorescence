
#include "manager.hpp"

#include <boost/filesystem/operations.hpp>

#include "renderqueue.hpp"
#include "cursormanager.hpp"
#include "doubleclickhandler.hpp"
#include "gumpmenu.hpp"
#include "gumpfactory.hpp"
#include "gumpmenus.hpp"
#include "gumpactions.hpp"

#include <client.hpp>

#include <misc/log.hpp>
#include <misc/exception.hpp>


#include <net/manager.hpp>
#include <net/packets/speechrequest.hpp>

namespace uome {
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
        throw Exception("uome::ui::Manager Singleton called before being created");
    }

    return singleton_;
}

Manager::Manager() {
    CL_OpenGLWindowDescription description;
    description.set_size(CL_Size(1024, 768), true);
    description.set_title("UO:ME");
    description.set_allow_resize(true);
    mainWindow_.reset(new CL_DisplayWindow(description));

    slotCloseWindow = mainWindow_->sig_window_close().connect(Client::getSingleton(), &Client::shutdown);

    windowManager_.reset(new CL_GUIWindowManagerTexture(*mainWindow_));

    boost::filesystem::path path = "themes";
    path = path / "default";
    guiManager_.reset(new CL_GUIManager(*windowManager_, path.string()));

    path = "fonts";
    loadFontDirectory(path);
}

bool Manager::setShardConfig(Config& config) {
    boost::filesystem::path path = "shards";
    path = path / config["/uome/shard@name"].asPath() / "themes" / config["/uome/ui/theme@name"].asPath();

    if (!boost::filesystem::exists(path)) {
        path = "themes";
        path = path / config["/uome/ui/theme@name"].asPath();

        if (!boost::filesystem::exists(path)) {
            LOG_EMERGENCY << "Unable to load theme, directory does not exist: " << config["/uome/ui/theme@name"].asPath() << std::endl;
            return false;
        }
    }

    guiManager_->exit_with_code(0);
    guiManager_.reset(new CL_GUIManager(*windowManager_, path.string()));

    path = "fonts";
    loadFontDirectory(path);

    path = "shards";
    path = path / config["/uome/shard@name"].asPath() / "fonts";
    loadFontDirectory(path);

    renderQueue_.reset(new RenderQueue());

    cursorManager_.reset(new CursorManager(config, mainWindow_));
    singleton_->getCursorManager()->setCursor(CursorType::GAME_WEST);

    doubleClickHandler_.reset(new DoubleClickHandler(config));
    doubleClickHandler_->start();

    return true;
}

Manager::~Manager() {
    LOG_INFO << "ui::Manager shutdown" << std::endl;
    if (renderQueue_) {
        renderQueue_->clear();
    }
}

void Manager::step() {
    windowManager_->process();

    getGraphicsContext().clear();
    windowManager_->draw_windows(getGraphicsContext());
    mainWindow_->flip(); // use parameter 1 here for vsync

    CL_KeepAlive::process();

    processCloseList();
}

boost::shared_ptr<CL_DisplayWindow> Manager::getMainWindow() {
    return singleton_->mainWindow_;
}

CL_GraphicContext& Manager::getGraphicsContext() {
    return singleton_->mainWindow_->get_gc();
}

CL_Texture* Manager::provideTexture(unsigned int width, unsigned int height) {
    return new CL_Texture(singleton_->getGraphicsContext(), width, height, cl_rgb8);
}

boost::shared_ptr<RenderQueue> Manager::getRenderQueue() {
    return singleton_->renderQueue_;
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

void Manager::closeGumpMenu(GumpMenu* menu) {
    closeListMutex_.lock();
    closeList_.push_back(menu);
    closeListMutex_.unlock();
}

void Manager::closeGumpMenu(const UnicodeString& gumpName) {
    boost::recursive_mutex::scoped_lock myLock(gumpListMutex_);

    GumpMenu* menu = getGumpMenu(gumpName);
    if (menu) {
        closeGumpMenu(menu);
    }
}

GumpMenu* Manager::getGumpMenu(const UnicodeString& gumpName) {
    boost::recursive_mutex::scoped_lock myLock(gumpListMutex_);

    std::list<GumpMenu*>::iterator iter = gumpList_.begin();
    std::list<GumpMenu*>::iterator end = gumpList_.end();

    for (; iter != end; ++iter) {
        if ((*iter)->getName() == gumpName) {
            return *iter;
        }
    }

    return NULL;
}

void Manager::processCloseList() {
    closeListMutex_.lock();
    gumpListMutex_.lock();

    std::list<GumpMenu*>::iterator iter = closeList_.begin();
    std::list<GumpMenu*>::iterator end = closeList_.end();

    for (; iter != end; ++iter) {
        delete (*iter);
        gumpList_.remove(*iter);
    }

    closeList_.clear();

    gumpListMutex_.unlock();
    closeListMutex_.unlock();
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

            CL_Font_System::register_font(iter->path().string(), iter->path().stem());
        }
    }
}

GumpMenu* Manager::openXmlGump(const UnicodeString& name) {
    return GumpFactory::fromXmlFile(name);
}

void Manager::registerGumpMenu(GumpMenu* menu) {
    gumpListMutex_.lock();
    gumpList_.push_back(menu);
    gumpListMutex_.unlock();
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

void Manager::systemMessage(const UnicodeString& msg) {
    // TODO: display, add to journal
    LOG_INFO << "SysMsg: " << msg << std::endl;
}

}
}
