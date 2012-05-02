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

#include "log.hpp"

#include <iomanip>

#include "exception.hpp"
#include <client.hpp>

namespace fluo {

Log* Log::singleton_ = NULL;

Log* Log::getSingleton() {
    if (!singleton_) {
        singleton_ = new Log();
    }
    return singleton_;
}

Log::Log() :
#ifdef WIN32
std::ostream(std::_Uninitialized()),
#endif
file_("fluorescence.log"), reportingLevel_(0) {
    levelStrings_[0] = " [DEBUG]: ";
    levelStrings_[1] = " [INFO ]: ";
    levelStrings_[2] = " [WARN ]: ";
    levelStrings_[3] = " [ERROR]: ";
    levelStrings_[4] = " [EMERG]: ";

    if (!file_.is_open()) {
        throw Exception("Unable to create log file");
    }

    std::cout.precision(4);
    file_.precision(4);
}

void Log::setLevel(unsigned int lvl) {
    reportingLevel_ = lvl;
}

void Log::close() {
    file_.close();
    std::cout.flush();
}

unsigned int Log::getReportingLevel() {
    return reportingLevel_;
}

Log& Log::prefix(unsigned int lvl) {
    *this << getCurrentRelativeTime() << getLevelString(lvl);
    return *this;
}

const char* Log::getLevelString(unsigned int lvl) const {
    if (lvl <= 4) {
        return levelStrings_[lvl];
    }
    return levelStrings_[4];
}

const char* Log::getCurrentRelativeTime() const {
    timeval t = Client::getSingleton()->getElapsedTime();

    static char timeBuf[64];
    sprintf(timeBuf, "%li.%06li ", (long int)t.tv_sec, (long int)t.tv_usec);

    return timeBuf;
}

Log& Log::operator<<(const struct timeval& v) {
    struct tm* ptm = localtime (reinterpret_cast<const time_t*>(&v.tv_sec));

    char buf[10];
    strftime(buf, 10, "%H:%M:%S", ptm);

    int msecs = v.tv_usec / 1000;

    *this << buf << "." << msecs;

    return *this;
}

Log& Log::operator<<(std::streambuf *buf) {
    file_<< buf;
    std::cout << buf;

    return *this;
}

Log& Log::operator<<(std::ios& ( *manip )(std::ios &)) {
    file_ << manip;
    std::cout << manip;

    return *this;
}

Log& Log::operator<<(std::ostream& ( *manip )(std::ostream &)) {
    file_ << manip;
    std::cout << manip;

    return *this;
}

Log& Log::operator<<(std::ios_base& ( *manip )(std::ios_base &)) {
    file_ << manip;
    std::cout << manip;

    return *this;
}

Log& Log::operator<<(const CL_Rectf& rect) {
    //return *this << "[Rect: " << std::fixed << std::setprecision(7) << rect.left << "/" << rect.top << "/" << rect.right << "/" << rect.bottom << "]";
    return *this << "[Rect: " << (int)rect.left << "/" << (int)rect.top << "/" << (int)rect.get_width() << "/" << (int)rect.get_height() << "]";
}

Log& Log::operator<<(const CL_Vec3f& vec) {
    return *this << "[Vec3: " << std::fixed << vec.x << "/" << vec.y << "/" << vec.z << "]";
}

Log& Log::operator<<(const CL_Vec4f& vec) {
    return *this << "[Vec4: " << std::fixed << vec.x << "/" << vec.y << "/" << vec.z << "/" << vec.w << "]";
}

}
