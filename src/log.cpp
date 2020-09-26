//
// Created by w703710691d on 18-8-24.
//
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <sys/time.h>
#include <cstdarg>
#include <unistd.h>
#include <sys/file.h>
#include "log.h"
#include <unistd.h>
#include <climits>
#include <sys/stat.h>

char LOG_LEVEL_NOTE[][10] = {
    "FATAL", "WARNING", "MONITOR", "NOTICE", "TRACE", "DEBUG"};

void PowerLogger::writeLog(int level, const char *file,
               const int line, const char *fmt, ...) {
    if (m_logLevel < level) {
        return;
    }
    if (!m_pFile) {
        m_pFile = fopen((m_path + m_fileName).c_str(), "a");
        if (!m_pFile) {
            perror("cannot open log file");
            return;
        }
    }
    char writeBuffer[LOG_BUFFER_SIZE];
    char logBuffer[LOG_BUFFER_SIZE];
    char datetimeBuffer[64];
    timeval tv{};
    if (gettimeofday(&tv, nullptr) == 0) {
        strftime(datetimeBuffer, sizeof(datetimeBuffer),
            "%Y-%m-%d %H:%M:%S", localtime(&tv.tv_sec));
    }

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(logBuffer, LOG_BUFFER_SIZE, fmt, ap);
    va_end(ap);

    int count = snprintf(writeBuffer, LOG_BUFFER_SIZE,
        "[%s]\t[%s.%06ld][#%d]<%s:%d>%s\n",
        LOG_LEVEL_NOTE[level],
        datetimeBuffer, tv.tv_usec,
        getpid(),
        file, line,
        logBuffer);

    if (level == FATAL) {
        fprintf(stderr, "%s\n", logBuffer);
    }

    int log_fd = m_pFile->_fileno;
    if (flock(log_fd, LOCK_EX) == 0) {
        if (write(log_fd, writeBuffer, count) < 0) {
            perror("write log error");
        }
        flock(log_fd, LOCK_UN);
    } else {
        perror("flock log file error");
    }
}

bool PowerLogger::setLogPath(std::string path) {
    if (access(path.c_str(), F_OK | W_OK | R_OK | X_OK)) {
        return false;
    }
    struct stat pathStat{};
    stat(path.c_str(), &pathStat);
    if (!S_ISDIR(pathStat.st_mode)){
        return false;
    }
    m_path = path;
    if (m_path.back() != '/') {
        m_path.append(1, '/');
    }
    return true;
}

void PowerLogger::setLogFileName(std::string fileName)
{
    if (m_pFile) {
        fclose(m_pFile);
    }
    m_fileName = fileName;
}

void PowerLogger::setLogLevel(int loglevel)
{
    m_logLevel = loglevel;
}

PowerLogger::PowerLogger() {
    m_pFile = nullptr;
    char path[PATH_MAX];
    if (readlink("/proc/self/exe", path, PATH_MAX) <= 0) {
        return;
    }
    char *sep = strrchr(path, '/');
    if (sep == nullptr) {
        return;
    }
    m_path = std::string(path, sep + 1);
    m_fileName = sep + 1;
}

PowerLogger::~PowerLogger()
{
    if (m_pFile) {
        fclose(m_pFile);
        m_pFile = nullptr;
    }
}

PowerLogger &PowerLogger::instance() {
    static PowerLogger logger;
    return logger;
}
