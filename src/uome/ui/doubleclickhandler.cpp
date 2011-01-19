
#include "doubleclickhandler.hpp"

#include <boost/thread.hpp>

#include <world/ingameobject.hpp>

#include <misc/logger.hpp>

namespace uome {
namespace ui {

DoubleClickHandler::DoubleClickHandler(const boost::program_options::variables_map& config) : running_(true), sema_(0) {
    timeoutMillis_ = config["ui.doubleclick-timeout-ms"].as<unsigned int>();
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
    while (running_) {
        sema_.wait();
        if (!running_) {
            break;
        }
        LOG_DEBUG(LOGTYPE_INPUT, "after first wait");

        boost::posix_time::ptime wakeTime = boost::posix_time::microsec_clock::universal_time() + boost::posix_time::milliseconds(timeoutMillis_);

        bool doubleClick = sema_.timed_wait(wakeTime);
        if (!running_) {
            break;
        }

        LOG_DEBUG(LOGTYPE_INPUT, "after timed wait");

        if (doubleClick) {
            LOG_DEBUG(LOGTYPE_INPUT, "dch doubleclick");
            lastObject_->onDoubleClick();
        } else {
            LOG_DEBUG(LOGTYPE_INPUT, "dch click");
            lastObject_->onClick();
        }

        lastObject_.reset();
    }
}

}
}
