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


#ifndef FLUO_UPDATER_UI_HPP
#define FLUO_UPDATER_UI_HPP

#include <ClanLib/application.h>
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/swrender.h>

#include <boost/thread/mutex.hpp>

#include "svnnotify.hpp"

namespace fluo {
namespace updater {

class Ui : public SvnNotify {
public:
    Ui();
    ~Ui();

    void step();

    void setMessage(const std::string& msg);
    void setError(const std::string& error);
    virtual void notify(const std::string& str);

    void close();

private:
    CL_SetupCore clSetupCore_;
    CL_SetupDisplay clSetupDisplay_;
    CL_SetupSWRender clSetupSWRender_;

    CL_DisplayWindow* mainWindow_;
    CL_Slot slotCloseWindow_;

    void drawStatus();
    void processInput();

    unsigned int blinkTimer_;

    CL_Font messageFont_;
    std::string message_;
    std::string error_;

    boost::mutex notificationMutex_;
    unsigned int notificationCount_;
    unsigned int notificationReadIdx_;
    unsigned int notificationWriteIdx_;

    static const unsigned int NOTIFY_COUNT = 25;
    std::string notifications_[NOTIFY_COUNT];
};

}
}

#endif

