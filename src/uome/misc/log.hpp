#ifndef UOME_MISC_LOG_HPP
#define UOME_MISC_LOG_HPP

#include <sys/time.h>
#include <string>
#include <fstream>
#include <iostream>
#include <unicode/ustream.h>

#include <boost/thread/mutex.hpp>


#define LOG_DEBUG \
if (uome::LOG_LEVEL_DEBUG < uome::Log::getSingleton()->getReportingLevel()) ; \
else uome::Log::getSingleton()->prefix(uome::LOG_LEVEL_DEBUG)

#define LOG_INFO \
if (uome::LOG_LEVEL_INFO < uome::Log::getSingleton()->getReportingLevel()) ; \
else uome::Log::getSingleton()->prefix(uome::LOG_LEVEL_INFO)

#define LOG_WARN \
if (uome::LOG_LEVEL_WARN < uome::Log::getSingleton()->getReportingLevel()) ; \
else uome::Log::getSingleton()->prefix(uome::LOG_LEVEL_WARN)

#define LOG_ERROR \
if (uome::LOG_LEVEL_ERROR < uome::Log::getSingleton()->getReportingLevel()) ; \
else uome::Log::getSingleton()->prefix(uome::LOG_LEVEL_ERROR)

#define LOG_EMERGENCY \
if (uome::LOG_LEVEL_EMERGENCY < uome::Log::getSingleton()->getReportingLevel()) ; \
else uome::Log::getSingleton()->prefix(uome::LOG_LEVEL_EMERGENCY)


#define LOG_INIT(level) uome::Log::getSingleton()->setLevel(level)
#define LOG_CLOSE uome::Log::getSingleton()->close()

namespace uome {

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
    Log& operator<<(const T& data) {
        file_ << data;
        std::cout << data;

        return *this;
    }

    Log& operator<<(std::streambuf *buf);
    Log& operator<<(std::ios& ( *manip )(std::ios &));
    Log& operator<<(std::ostream& ( *manip )(std::ostream &));
    Log& operator<<(std::ios_base& ( *manip )(std::ios_base &));
    Log& operator<<(const struct timeval& v);

private:
    Log();
    Log(const Log& copy) { }
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
