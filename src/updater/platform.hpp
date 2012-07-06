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


#ifndef FLUO_PLATFORM_HPP
#define FLUO_PLATFORM_HPP

// workaround for gettimeofday on windows
// written by http://suacommunity.com/dictionary/gettimeofday-entry.php
#ifdef WIN32

#include <time.h>

// necessary because compiling fails somewhere in aes due to a redefinition of a typedef
#define NOCRYPT
#include <windows.h>
#undef NOCRYPT


struct timezone
{
  int  tz_minuteswest; /* minutes W of Greenwich */
  int  tz_dsttime;     /* type of dst correction */
};

int gettimeofday(struct timeval *tv, struct timezone *tz);

#else

#include <sys/time.h>

#endif


// helper to sleep a certain amount of milliseconds
namespace fluo {
    void sleepMs(unsigned int ms);
}

#endif
