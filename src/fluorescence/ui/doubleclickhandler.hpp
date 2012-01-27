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


#ifndef FLUO_UI_DOUBLECLICKHANDLER_HPP
#define FLUO_UI_DOUBLECLICKHANDLER_HPP

#include <boost/shared_ptr.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>

#include <misc/config.hpp>

namespace boost {
    class thread;
}

namespace fluo {

namespace world {
    class IngameObject;
}

namespace ui {

class Manager;

/***
 * This class is used to distinguish double clicks from single clicks.
 * On win, this might not be necessary, but there is no double click support in clanlib
 * linux code.
 * Runs a thread that wait()s for a semaphore to be called (happens at each click on
 * an ingameobject). After the first wait, there is a timewait with the doubleclick
 * timeout. If another post() is made => double click, otherwise => single click
 */
class DoubleClickHandler {

friend class ui::Manager;

public:
    DoubleClickHandler(Config& config);
    ~DoubleClickHandler();

    void start();
    void kill();
    void run();

private:
    bool running_;

    boost::interprocess::interprocess_semaphore sema_;
    unsigned int timeoutMillis_;

    boost::shared_ptr<boost::thread> thread_;

    boost::shared_ptr<world::IngameObject> lastObject_;

    void notify(boost::shared_ptr<world::IngameObject> obj);
};

}
}

#endif
