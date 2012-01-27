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



#include "doubleclickhandler.hpp"

#include <boost/thread.hpp>

#include <misc/log.hpp>

#include "manager.hpp"

namespace fluo {
namespace ui {

DoubleClickHandler::DoubleClickHandler(Config& config) : running_(true), sema_(0) {
    timeoutMillis_ = config["/fluo/input/mouse@doubleclick-timeout-ms"].asInt();
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
