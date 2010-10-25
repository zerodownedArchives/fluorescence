#ifndef UOME_LOGGER_HPP
#define UOME_LOGGER_HPP

#include <stdio.h>
#include <sys/time.h>

#include <boost/thread/mutex.hpp>

#include <unicode/unistr.h>
extern "C" {
#include <unicode/ustdio.h>
}

namespace uome {

#define DEFAULT_LOGFILE "uome.log"

#define LOGSEVERITY_DEBUG    0
#define LOGSEVERITY_INFO     1
#define LOGSEVERITY_WARN     2
#define LOGSEVERITY_ERROR    3
#define LOGSEVERITY_CRITICAL 4

#define MINIMUM_LOGSEVERITY LOGSEVERITY_DEBUG


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
    virtual ~Logger() {
        boost::mutex::scoped_lock myLock(mutex_);
        u_fclose(logFile_);
        u_fclose(cout_);
    }

    //dont use this functions. use the macros instead
    void init(unsigned int type, char* filename = DEFAULT_LOGFILE) {
        boost::mutex::scoped_lock myLock(mutex_);
        type_ = type;
        u_fclose(logFile_);
        logFile_ = u_finit(fopen(filename, "w"), NULL, NULL);
        if (!logFile_) {
            fprintf(stderr, "Warning: Could not create Logfile!\n");
        }

        LOG_INFO(LOGTYPE_UNKNOWN, "Logger Re-initialized");
    }

    inline static Logger* getInstance() {
        static Logger* instance = NULL;
        if (!instance) {
            instance = new Logger(LOGTYPE_ALL, DEFAULT_LOGFILE);
        }

        return instance;
    }

    void logMessage(unsigned int type, const char* message) {
        if ((type & type_) == 0) {
            return;
        }

        boost::mutex::scoped_lock myLock(mutex_);

        printTime();
        printType(type);

        u_fprintf(cout_, "%s\n", message);
        u_fflush(cout_);
        u_fprintf(logFile_, "%s\n", message);
        u_fflush(logFile_);
    }

    void logArgMessage(unsigned int type, const char* message...) {
        if ((type & type_) == 0)
            return;

        boost::mutex::scoped_lock myLock(mutex_);

        printTime();
        printType(type);

        va_list arg;
        va_start(arg, message);

        u_vfprintf(cout_, message, arg);
        u_fprintf(cout_, "\n");
        u_fflush(cout_);

        u_vfprintf(logFile_, message, arg);
        u_fprintf(logFile_, "\n");
        u_fflush(logFile_);

        va_end(arg);
    }

private:
    unsigned int type_;
    char* typeText_;
    UFILE* cout_;
    UFILE* logFile_;

    timeval startTime_;

    boost::mutex mutex_;

    Logger(unsigned int type, const char* filename) : type_(type) {
        gettimeofday(&startTime_, NULL);
        logFile_ = u_finit(fopen(filename, "w"), NULL, NULL);
        if (!logFile_) {
            fprintf(stderr, "Warning: Could not create Logfile!\n");
        }

        cout_ = u_finit(stdout, NULL, NULL);
    }

    void printTime() {
        timeval t = getCurrentRelativeTime();
        char timeBuf[20];
        sprintf(timeBuf, "%05li.%06li ", (long int)t.tv_sec, (long int)t.tv_usec);

        u_fprintf(cout_, "%s", timeBuf);
        u_fprintf(logFile_, "%s", timeBuf);
    }

    void printType(unsigned int type) {
        const char* pTypeText = getTypeText(type);
        u_fprintf(cout_, "%-6s", pTypeText);
        u_fprintf(logFile_, "%-6s", pTypeText);
    }

    const char* getTypeText(unsigned int type) {
        switch (type) {
        case LOGTYPE_MAIN:
            return "MAIN: ";
        case LOGTYPE_INPUT:
            return "INPUT: ";
        case LOGTYPE_NETWORK:
            return "NET: ";
        case LOGTYPE_RENDERER:
            return "RNDR: ";
        case LOGTYPE_DATA:
            return "DATA: ";
        case LOGTYPE_UI:
            return "UI: ";
        case LOGTYPE_SOUND:
            return "SOUND: ";
        case LOGTYPE_WORLD:
            return "WORLD: ";
        case LOGTYPE_PRERENDERER:
            return "PRNDR: ";
        case LOGTYPE_CONFIG:
            return "CONF: ";
        case LOGTYPE_PYTHON:
            return "PY: ";
        case LOGTYPE_UNKNOWN:
        default:
            return "UOME: ";
        }
    }

    timeval getCurrentRelativeTime() {
        timeval cur;
        gettimeofday(&cur, NULL);

        timeval ret;
        ret.tv_sec = cur.tv_sec - startTime_.tv_sec;

        if (cur.tv_usec >= startTime_.tv_usec) {
            ret.tv_usec = cur.tv_usec - startTime_.tv_usec;
        } else {
            ret.tv_sec -= 1;
            ret.tv_usec = (cur.tv_usec + 1000000) - startTime_.tv_usec;
        }

        return ret;
    }
};

}

#endif
