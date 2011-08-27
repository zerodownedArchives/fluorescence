
#include "doubleclickhandler.hpp"

#include <boost/thread.hpp>

#include <misc/log.hpp>

#include "manager.hpp"

namespace uome {
namespace ui {

DoubleClickHandler::DoubleClickHandler(Config& config) : running_(true), sema_(0) {
    timeoutMillis_ = config["/uome/input/mouse@doubleclick-timeout-ms"].asInt();
}

DoubleClickHandler::~DoubleClickHandler() {
    if (thread_) {
        kill();
        sema_.post();
        thread_->join();
    }
}

void DoubleClickHandler::notify(boost::shared_ptr<world::IngameObject> obj) {
    lastObject_ = obj;
    sema_.post();
}

void DoubleClickHandler::start() {
    running_ = true;
    thread_.reset(new boost::thread(boost::bind(&DoubleClickHandler::run, this)));
}

void DoubleClickHandler::kill() {
    running_ = false;
}

void DoubleClickHandler::run() {
    ui::Manager* uiMan = ui::Manager::getSingleton();

    while (running_) {
        sema_.wait();
        if (!running_) {
            break;
        }
        //LOG_DEBUG(LOGTYPE_INPUT, "after first wait");

        boost::posix_time::ptime wakeTime = boost::posix_time::microsec_clock::universal_time() + boost::posix_time::milliseconds(timeoutMillis_);

        bool doubleClick = sema_.timed_wait(wakeTime);
        if (!running_) {
            break;
        }

        //LOG_DEBUG(LOGTYPE_INPUT, "after timed wait");

        if (doubleClick) {
            //LOG_DEBUG(LOGTYPE_INPUT, "dch doubleclick");
            uiMan->queueDoubleClick(lastObject_);
        } else {
            //LOG_DEBUG(LOGTYPE_INPUT, "dch click");
            uiMan->queueSingleClick(lastObject_);
        }

        lastObject_.reset();
    }
}

}
}
