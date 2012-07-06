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

#ifndef FLUO_UPDATER_UPDATER_HPP
#define FLUO_UPDATER_UPDATER_HPP

#include <vector>
#include <ClanLib/Core/Text/string8.h>
#include <boost/thread/thread.hpp>

#include <platform.hpp>

namespace fluo {
namespace updater {

class Ui;
class Svn;

class Updater {
public:
    static int sMain(const std::vector<CL_String8>& args);
    static Updater* getSingleton();

    int main(const std::vector<CL_String8>& args);

    timeval getElapsedTime() const;

    void close();
    void setRPressed(bool value);

private:
    static Updater* singleton_;

    Updater();
    ~Updater();

    void init();

    timeval startTime_;

    Ui* ui_;
    Svn* svn_;

    boost::thread svnThread_;
    void svnThreadFunc();
    void svnRevertFunc();

    bool shouldExit_;

    bool hasCheckoutError_;
    bool hasUpdateError_;
    bool hasRevertError_;

    bool rPressed_;
};

}
}

#endif
