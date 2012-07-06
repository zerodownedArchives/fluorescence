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

#include "ui.hpp"

#include "updater.hpp"

namespace fluo {
namespace updater {

#ifndef WIN32
// gcc wants this here, too. visual studio complains...
const unsigned int Ui::NOTIFY_COUNT;
#endif

Ui::Ui() : blinkTimer_(0), notificationCount_(0), notificationWriteIdx_(0) {
    CL_DisplayWindowDescription description;
    description.set_size(CL_Size(400, 600), false);
    description.set_title("fluorescence Updater");
    description.set_allow_resize(true);
    mainWindow_ = new CL_DisplayWindow(description);

    slotCloseWindow_ = mainWindow_->sig_window_close().connect(Updater::getSingleton(), &Updater::close);

#ifdef WIN32
	unsigned int fontSize = 14;
#else
	unsigned int fontSize = 12;
#endif
    messageFont_ = CL_Font(mainWindow_->get_gc(), "Courier new", fontSize);
}

Ui::~Ui() {
    delete mainWindow_;
}

void Ui::step() {
    CL_KeepAlive::process();

    ++blinkTimer_;

    drawStatus();
    processInput();

    mainWindow_->flip(1); // use parameter 1 here for vsync
}

void Ui::drawStatus() {
    CL_GraphicContext& gc = mainWindow_->get_gc();

    gc.clear();

    if (error_.length() > 0) {
        messageFont_.draw_text(gc, 20, 40, error_, CL_Colorf::red);
    } else {
        std::string dispMsg = message_;
        if (blinkTimer_ >= 60 && blinkTimer_ < 120) {
            dispMsg += " ...";
        } else if (blinkTimer_ >= 120) {
            blinkTimer_ = 0;
        }

        messageFont_.draw_text(gc, 20, 40, dispMsg, CL_Colorf::green);
    }

    boost::mutex::scoped_lock myLock(notificationMutex_);
    for (unsigned int i = 0; i < notificationCount_; ++i) {
        int idx = (notificationReadIdx_ + i) % NOTIFY_COUNT;
        messageFont_.draw_text(gc, 20, 60 + i*20, notifications_[idx], CL_Colorf::grey);
    }
}

void Ui::processInput() {
    CL_InputDevice keyboard = mainWindow_->get_ic().get_keyboard();
    if (keyboard.get_keycode(CL_KEY_ESCAPE)) {
        Updater::getSingleton()->close();
    }

    if (keyboard.get_keycode(CL_KEY_R)) {
        Updater::getSingleton()->setRPressed(true);
    } else {
        Updater::getSingleton()->setRPressed(false);
    }
}

void Ui::setMessage(const std::string& msg) {
    message_ = msg;
    notify(msg);
}

void Ui::setError(const std::string& error) {
    error_ = error;
    notify(error);
}

void Ui::notify(const std::string& str) {
    boost::mutex::scoped_lock myLock(notificationMutex_);
    notifications_[notificationWriteIdx_] = str;
    notificationWriteIdx_ = (notificationWriteIdx_ + 1) % NOTIFY_COUNT;
    notificationCount_ = (std::min)(NOTIFY_COUNT, notificationCount_ + 1);
    notificationReadIdx_ = notificationCount_ == NOTIFY_COUNT ? notificationWriteIdx_ : 0;
}

}
}

