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



#include "platform.hpp"


#ifdef WIN32

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

// Definition of a gettimeofday function

int gettimeofday(struct timeval *tv, struct timezone *tz) {
// Define a structure to receive the current Windows filetime
    FILETIME ft;

// Initialize the present time to 0 and the timezone to UTC
    unsigned __int64 tmpres = 0;
    static int tzflag = 0;

    if (NULL != tv) {
        GetSystemTimeAsFileTime(&ft);

// The GetSystemTimeAsFileTime returns the number of 100 nanosecond
// intervals since Jan 1, 1601 in a structure. Copy the high bits to
// the 64 bit tmpres, shift it left by 32 then or in the low 32 bits.
        tmpres |= ft.dwHighDateTime;
        tmpres <<= 32;
        tmpres |= ft.dwLowDateTime;

// Convert to microseconds by dividing by 10
        tmpres /= 10;

// The Unix epoch starts on Jan 1 1970.  Need to subtract the difference
// in seconds from Jan 1 1601.
        tmpres -= DELTA_EPOCH_IN_MICROSECS;

// Finally change microseconds to seconds and place in the seconds value.
// The modulus picks up the microseconds.
        tv->tv_sec = (long)(tmpres / 1000000UL);
        tv->tv_usec = (long)(tmpres % 1000000UL);
    }

    if (NULL != tz) {
        if (!tzflag) {
            _tzset();
            tzflag++;
        }

// Adjust for the timezone west of Greenwich
        tz->tz_minuteswest = _timezone / 60;
        tz->tz_dsttime = _daylight;
    }

    return 0;
}

#endif


#ifndef WIN32
#include <unistd.h>
#endif


namespace fluo {
    void sleepMs(unsigned int ms) {
#ifdef WIN32
        Sleep(ms);
#else
        usleep(ms * 1000);
#endif
    }
}
