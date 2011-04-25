
#include "logger.hpp"

namespace uome {

Logger::~Logger() {
    boost::mutex::scoped_lock myLock(mutex_);
    u_fclose(logFile_);
    u_fclose(cout_);
}

void Logger::init(unsigned int type, const char* filename) {
    boost::mutex::scoped_lock myLock(mutex_);
    type_ = type;
    u_fclose(logFile_);
    logFile_ = u_finit(fopen(filename, "w"), NULL, NULL);
    if (!logFile_) {
        fprintf(stderr, "Warning: Could not create Logfile!\n");
    }

    LOG_INFO(LOGTYPE_UNKNOWN, "Logger Re-initialized");
}

Logger* Logger::getInstance() {
    static Logger* instance = NULL;
    if (!instance) {
        instance = new Logger(LOGTYPE_ALL, DEFAULT_LOGFILE);
    }

    return instance;
}

void Logger::logMessage(unsigned int type, const char* message) {
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

void Logger::logArgMessage(unsigned int type, const char* message...) {
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

    va_end(arg);
    va_start(arg, message);

    u_vfprintf(logFile_, message, arg);
    u_fprintf(logFile_, "\n");
    u_fflush(logFile_);

    va_end(arg);
}
Logger::Logger(unsigned int type, const char* filename) : type_(type) {
    gettimeofday(&startTime_, NULL);
    logFile_ = u_finit(fopen(filename, "w"), NULL, NULL);
    if (!logFile_) {
        fprintf(stderr, "Warning: Could not create Logfile!\n");
    }

    cout_ = u_finit(stdout, NULL, NULL);
}

void Logger::printTime() {
    timeval t = getCurrentRelativeTime();
    char timeBuf[20];
    sprintf(timeBuf, "%05li.%06li ", (long int)t.tv_sec, (long int)t.tv_usec);

    u_fprintf(cout_, "%s", timeBuf);
    u_fprintf(logFile_, "%s", timeBuf);
}

void Logger::printType(unsigned int type) {
    const char* pTypeText = getTypeText(type);
    u_fprintf(cout_, "%-6s", pTypeText);
    u_fprintf(logFile_, "%-6s", pTypeText);
}

const char* Logger::getTypeText(unsigned int type) {
    switch (type) {
    case LOGTYPE_MAIN:
        return "MAIN: ";
    case LOGTYPE_INPUT:
        return "INPT: ";
    case LOGTYPE_NETWORK:
        return "NET:  ";
    case LOGTYPE_RENDERER:
        return "RNDR: ";
    case LOGTYPE_DATA:
        return "DATA: ";
    case LOGTYPE_UI:
        return "UI:   ";
    case LOGTYPE_SOUND:
        return "SND:  ";
    case LOGTYPE_WORLD:
        return "WRLD: ";
    case LOGTYPE_CONFIG:
        return "CONF: ";
    case LOGTYPE_PYTHON:
        return "PY:   ";
    case LOGTYPE_UNKNOWN:
    default:
        return "UOME: ";
    }
}

timeval Logger::getCurrentRelativeTime() {
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

}
