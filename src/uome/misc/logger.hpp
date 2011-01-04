#ifndef UOME_LOGGER_HPP
#define UOME_LOGGER_HPP

#include <stdio.h>
#include <sys/time.h>

#include <boost/thread/mutex.hpp>

#include "string.hpp"

namespace uome {

static const char* DEFAULT_LOGFILE = "uome.log";

static const unsigned int LOGSEVERITY_DEBUG     = 0;
static const unsigned int LOGSEVERITY_INFO      = 1;
static const unsigned int LOGSEVERITY_WARN      = 2;
static const unsigned int LOGSEVERITY_ERROR     = 3;
static const unsigned int LOGSEVERITY_CRITICAL  = 4;

static const unsigned int MINIMUM_LOGSEVERITY   = LOGSEVERITY_DEBUG;


#define LOGGER_INIT(type, pFilename)         (Logger::getInstance->init(type, _pFilename != 0 ? _pFilename : DEFAULT_LOGFILE))
#define LOGGER_QUIT()                        (Logger::quit())
#define LOG(type, message)                   (Logger::getInstance()->logMessage(type, message))
#define LOGARG(type, message, ...)           (Logger::getInstance()->logArgMessage(type, message, __VA_ARGS__))

#if (MINIMUM_LOGSEVERITY <= LOGSEVERITY_DEBUG)
#define LOG_DEBUG(type, message)             LOG(type, message)
#define LOGARG_DEBUG(type, message, ...)     LOGARG(type, message, __VA_ARGS__)
#else
#define LOG_DEBUG(type, message)             {}
#define LOGARG_DEBUG(type, message, ...)     {}
#endif


#if (MINIMUM_LOGSEVERITY <= LOGSEVERITY_INFO)
#define LOG_INFO(type, message)              LOG(type, message)
#define LOGARG_INFO(type, message, ...)      LOGARG(type, message, __VA_ARGS__)
#else
#define LOG_INFO(type, message)              {}
#define LOGARG_INFO(type, message, ...)      {}
#endif

#if (MINIMUM_LOGSEVERITY <= LOGSEVERITY_WARN)
#define LOG_WARN(type, message)              LOG(type, message)
#define LOGARG_WARN(type, message, ...)      LOGARG(type, message, __VA_ARGS__)
#else
#define LOG_WARN(type, message)              {}
#define LOGARG_WARN(type, message)           {}
#endif

#if (MINIMUM_LOGSEVERITY <= LOGSEVERITY_ERROR)
#define LOG_ERROR(type, message)             LOG(type, message)
#define LOGARG_ERROR(type, message, ...)     LOGARG(type, message, __VA_ARGS__)
#else
#define LOG_ERROR(type, message)             {}
#define LOGARG_ERROR(type, message)          {}
#endif

#if (MINIMUM_LOGSEVERITY <= LOGSEVERITY_CRITICAL)
#define LOG_CRITICAL(type, message)          LOG(type, message)
#define LOGARG_CRITICAL(type, message, ...)  LOGARG(type, message, __VA_ARGS__)
#else
#define LOG_CRITICAL(type, message)          {}
#define LOGARG_CRITICAL(type, message)       {}
#endif


enum LOGTYPE {
    LOGTYPE_UNKNOWN     = 1 << 0,
    LOGTYPE_MAIN        = 1 << 1,
    LOGTYPE_INPUT       = 1 << 2,
    LOGTYPE_NETWORK     = 1 << 3,
    LOGTYPE_RENDERER    = 1 << 4,
    LOGTYPE_DATA        = 1 << 5,
    LOGTYPE_UI          = 1 << 6,
    LOGTYPE_SOUND       = 1 << 7,
    LOGTYPE_WORLD       = 1 << 8,

    LOGTYPE_PRERENDERER = 1 << 9,
    LOGTYPE_CONFIG      = 1 << 10,
    LOGTYPE_PYTHON      = 1 << 11,

    LOGTYPE_ALL         = 0xFFFFFFFF
};


class Logger {
public:
    static Logger* getInstance();

    virtual ~Logger();

    //dont use this functions. use the macros instead
    void init(unsigned int type, const char* filename = DEFAULT_LOGFILE);

    void logMessage(unsigned int type, const char* message);
    void logArgMessage(unsigned int type, const char* message...);

private:
    unsigned int type_;
    char* typeText_;
    UFILE* cout_;
    UFILE* logFile_;

    timeval startTime_;

    boost::mutex mutex_;

    Logger(unsigned int type, const char* filename);

    void printTime();
    void printType(unsigned int type);

    const char* getTypeText(unsigned int type);

    timeval getCurrentRelativeTime();
};

}

#endif
