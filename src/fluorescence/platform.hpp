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
