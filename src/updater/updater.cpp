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

#include "updater.hpp"

#include "svn.hpp"
#include "ui.hpp"
#include "misc/log.hpp"

namespace fluo {
namespace updater {

Updater* Updater::singleton_ = nullptr;

Updater* Updater::getSingleton() {
    return singleton_;
}

int Updater::sMain(const std::vector<CL_String8>& args) {
    singleton_ = new Updater();
    singleton_->init();
    LOG_INIT(0);

    int ret = singleton_->main(args);

    LOG_CLOSE;
    delete singleton_;
    return ret;
}

Updater::Updater() : ui_(nullptr), svn_(nullptr), shouldExit_(false),
    hasCheckoutError_(false), hasUpdateError_(false), hasRevertError_(false), patcherUpdate_(false) {
    gettimeofday(&startTime_, NULL);
}

void Updater::init() {
    ui_ = new Ui();

    const char* directory = ".";
    const char* url = "http://fluorescence-dist.googlecode.com/svn/branches/stable/";
    svn_ = new Svn(directory, url, ui_);
}

Updater::~Updater() {
    delete ui_;
    delete svn_;
}

int Updater::main(const std::vector<CL_String8>& args) {
    ui_->setMessage("Initializing");

    svnThread_ = boost::thread(boost::bind(&Updater::svnThreadFunc, this));

    while (!svnThread_.timed_join(boost::get_system_time())) {
        ui_->step();
    }

    if (hasCheckoutError_) {
        // display error, wait for user to exit program. show hints how to resolve the problem
        ui_->notify("");
        ui_->notify("");
        ui_->notify("Please use the installer/updater in an empty directory!");
        while (!shouldExit_) {
            ui_->step();
        }
    } else if (hasUpdateError_) {
        ui_->notify("");
        ui_->notify("");
        ui_->notify("Update might fail due to local modifications");
        ui_->notify("You can either: ");
        ui_->notify("- check manually for modifications ");
        ui_->notify("    and restart the updater. (The updater.log");
        ui_->notify("    file might provide some help on where to look)");
        ui_->notify("- or press r to revert automatically");
        ui_->notify("    (This overrides local changes)");

        while (!shouldExit_ && !rPressed_) {
            ui_->step();
        }
    }

    if (!shouldExit_) {
        if (hasUpdateError_ && rPressed_) {
            hasUpdateError_ = false;
            svnThread_ = boost::thread(boost::bind(&Updater::svnRevertFunc, this));

            while (!svnThread_.timed_join(boost::get_system_time())) {
                ui_->step();
            }

            if (hasRevertError_ || hasUpdateError_) {
                while (!shouldExit_) {
                    ui_->step();
                }
            }
        }
    }

    if (!hasRevertError_ && !hasUpdateError_ && !hasCheckoutError_ && !shouldExit_) {
        // start the client

        LOG_INFO << "Starting the client. Patcher update: " << patcherUpdate_ << std::endl;

#ifdef WIN32
        if (patcherUpdate_) {
            execl("fluorescence_win32.exe", "fluorescence_win32.exe", "--patcherupdate", nullptr);
        } else {
            execl("fluorescence_win32.exe", "fluorescence_win32.exe", nullptr);
        }
#else
        if (patcherUpdate_) {
            execl("./fluorescence", "./fluorescence", "--patcherupdate", nullptr);
        } else {
            execl("./fluorescence", "./fluorescence", nullptr);
        }
#endif

        // if we can execute this, something went wrong...
        ui_->setError("Unable to start fluorescence. Please restart!");
        ui_->notify(strerror(errno));
        while (!shouldExit_) {
            ui_->step();
        }
    }

    return 0;
}

timeval Updater::getElapsedTime() const {
    timeval cur;
    gettimeofday(&cur, NULL);

    timeval t;
    t.tv_sec = cur.tv_sec - startTime_.tv_sec;

    if (cur.tv_usec >= startTime_.tv_usec) {
        t.tv_usec = cur.tv_usec - startTime_.tv_usec;
    } else {
        t.tv_sec -= 1;
        t.tv_usec = (cur.tv_usec + 1000000) - startTime_.tv_usec;
    }

    return t;
}

void Updater::svnThreadFunc() {
    if (svn_->checkWorkingCopy()) {
        // is a working copy, only need to update
        LOG_INFO << "Working copy found, performing update..." << std::endl;
        ui_->setMessage("Working copy found, performing update");

        int rev = svn_->update();
        if (rev != -1) {
            std::stringstream sstr;
            sstr << "Successfully updated to revision " << rev;
            LOG_INFO << sstr.str() << std::endl;
            ui_->setMessage(sstr.str());
        } else {
            LOG_ERROR << "Update not successful" << std::endl;
            ui_->setError("Update not successful");
            hasUpdateError_ = true;
        }
    } else {
        LOG_INFO << "No working copy found, performing checkout..." << std::endl;
        ui_->setMessage("No working copy found, performing checkout");
        int rev = svn_->checkout();
        if (rev != -1) {
            std::stringstream sstr;
            sstr << "Successfully checked out revision " << rev;
            LOG_INFO << sstr.str() << std::endl;
            ui_->setMessage(sstr.str());
        } else {
            LOG_ERROR << "Checkout not successful" << std::endl;
            ui_->setError("Checkout not successful");
            hasCheckoutError_ = true;
        }
    }
}

void Updater::svnRevertFunc() {
    ui_->setError("");
    ui_->setMessage("Performing revert");
    LOG_INFO << "Performing revert operation..." << std::endl;

    if (svn_->revert()) {
        // revert went well, try to update again
        ui_->setMessage("Revert OK, performing update");
        LOG_INFO << "Revert OK, performing update..." << std::endl;

        int rev = svn_->update();
        if (rev != -1) {
            std::stringstream sstr;
            sstr << "Successfully updated to revision " << rev;
            LOG_INFO << sstr.str() << std::endl;
            ui_->setMessage(sstr.str());
        } else {
            LOG_ERROR << "Update not successful" << std::endl;
            ui_->setError("Update not successful");
            hasUpdateError_ = true;
        }
    } else {
        LOG_ERROR << "Revert not successful" << std::endl;
        ui_->setError("Revert not successful");
        hasRevertError_ = true;
    }
}

void Updater::close() {
    ui_->setMessage("Cancelling");
    shouldExit_ = true;
    svn_->cancel();
}

void Updater::setRPressed(bool value) {
    rPressed_ = value;
}

void Updater::patcherUpdate() {
    patcherUpdate_ = true;
}

}
}
