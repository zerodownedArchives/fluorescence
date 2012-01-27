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


#ifndef FLUO_MISC_LOG_HPP
#define FLUO_MISC_LOG_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <unicode/ustream.h>

#include <boost/thread/mutex.hpp>

#include "../platform.hpp"


#define LOG_DEBUG \
if (fluo::LOG_LEVEL_DEBUG < fluo::Log::getSingleton()->getReportingLevel()) ; \
else fluo::Log::getSingleton()->prefix(fluo::LOG_LEVEL_DEBUG)

#define LOG_INFO \
if (fluo::LOG_LEVEL_INFO < fluo::Log::getSingleton()->getReportingLevel()) ; \
else fluo::Log::getSingleton()->prefix(fluo::LOG_LEVEL_INFO)

#define LOG_WARN \
if (fluo::LOG_LEVEL_WARN < fluo::Log::getSingleton()->getReportingLevel()) ; \
else fluo::Log::getSingleton()->prefix(fluo::LOG_LEVEL_WARN)

#define LOG_ERROR \
if (fluo::LOG_LEVEL_ERROR < fluo::Log::getSingleton()->getReportingLevel()) ; \
else fluo::Log::getSingleton()->prefix(fluo::LOG_LEVEL_ERROR)

#define LOG_EMERGENCY \
if (fluo::LOG_LEVEL_EMERGENCY < fluo::Log::getSingleton()->getReportingLevel()) ; \
else fluo::Log::getSingleton()->prefix(fluo::LOG_LEVEL_EMERGENCY)


#define LOG_INIT(level) fluo::Log::getSingleton()->setLevel(level)
#define LOG_CLOSE fluo::Log::getSingleton()->close()

namespace fluo {

enum LogLevel {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_ERROR = 3,
    LOG_LEVEL_EMERGENCY = 4,
};

class Log : public std::ostream {
public:
    static Log* getSingleton();

    unsigned int getReportingLevel();

    void setLevel(unsigned int lvl);

    void close();

    /** \brief Prints the logging prefix to the streams. Prefix contains log level and current time */
    Log& prefix(unsigned int lvl);

    template<typename T>
    Log& operator<<(const T& val) {
        file_ << val;
        std::cout << val;

        return *this;
    }

    Log& operator<<(std::streambuf *buf);
    Log& operator<<(std::ios& ( *manip )(std::ios &));
    Log& operator<<(std::ostream& ( *manip )(std::ostream &));
    Log& operator<<(std::ios_base& ( *manip )(std::ios_base &));
    Log& operator<<(const struct timeval& v);

private:
    Log();

    // not overly proper, but otherwise visual studio cries about having no default ctor for ostream
#ifndef WIN32
    Log(const Log& copy) { }
#endif
    Log& operator=(const Log& copy) { return *this; }

    static Log* singleton_;

    std::ofstream file_;

    unsigned int reportingLevel_;

    const char* levelStrings_[5];

    timeval startTime_;
    const char* getCurrentRelativeTime() const;

    /** \brief Returns a string representation of the level */
    const char* getLevelString(unsigned int lvl) const;
};

}

#endif
