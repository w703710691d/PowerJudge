#include <cstring>
#include <cstdio>
#include <ctime>
#include <cstdarg>
#include <unistd.h>
#include <sys/file.h>
#include "powerlog.h"
#include <sys/time.h>
#include <fstream>
#include <list>

char LOG_LEVEL_NOTE[][10] = {
    "FATAL  ",
    "WARNING",
    "MONITOR",
    "NOTICE ",
    "TRACE  ",
    "DEBUG  "};

const size_t LOG_BUFFER_SIZE = 8192;

void PowerLogger::writeLog(int level, const char *file,
                           int line, const char *fmt, ...) {
    if (m_logLevel < level) {
        return;
    }
    if (!m_pFile && !_createLog()) {
        return;
    }
    if(_isLogTimeOut() && !_createLog()) {
        return;
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
        "[%s][%s.%06ld][#%d]<%s:%d>%s\n",
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

bool PowerLogger::setLogDir(const std::string &strLogDir) {
    std::filesystem::path logDirPath(strLogDir);
    if(!std::filesystem::exists(logDirPath) &&
        !std::filesystem::create_directories(logDirPath)) {
        return false;
    }
    std::filesystem::directory_entry dir(logDirPath);
    if(!dir.is_directory()) {
        return false;
    }
    if(m_pFile) {
        fclose(m_pFile);
        m_pFile = nullptr;
    }
    m_logDirPath = logDirPath;
    _clearLog();
    return true;
}

void PowerLogger::setLogLevel(int loglevel) {
    m_logLevel = loglevel;
}

PowerLogger::PowerLogger()
    : m_pFile(nullptr)
    , m_bizName("PowerLog")
    , m_logDirPath(".")
    , m_logCycle(7)
    , m_logLevel(PowerLogger::NOTICE){
}

PowerLogger::~PowerLogger(){
    if (m_pFile) {
        fclose(m_pFile);
        m_pFile = nullptr;
    }
}

PowerLogger &PowerLogger::instance() {
    static PowerLogger logger;
    return logger;
}

void PowerLogger::setBizName(const std::string &bizName) {
    m_bizName = bizName;
    if(m_pFile) {
        fclose(m_pFile);
        m_pFile = nullptr;
    }
    _clearLog();
}

void PowerLogger::setLogCycle(int days) {
    m_logCycle = days;
    _clearLog();
}

void PowerLogger::_clearLog() {
    std::filesystem::directory_iterator fileList(m_logDirPath);
    std::list<std::filesystem::path> removeList;
    for(auto &file : fileList) {
        if(file.status().type() ==
            std::filesystem::file_type::regular) {
            std::string fileName = file.path().filename();
            if(fileName.compare(0, m_bizName.size(), m_bizName) == 0 &&
                fileName.compare(fileName.size() - m_bizName.size(), m_bizName.size(), m_bizName) == 0 &&
                fileName.length() == m_bizName.size() + 4)
            {
                std::string logTimeStr = fileName.substr(m_bizName.size(), 8);
                int year = std::stoi(logTimeStr.substr(0, 4));
                int month = std::stoi(logTimeStr.substr(4, 2));
                int day = std::stoi(logTimeStr.substr(6, 2));
                tm fileTm{.tm_mday = day, .tm_mon = month - 1, .tm_year = year - 1900};
                time_t fileTime = mktime(&fileTm);
                time_t nowTime = time(nullptr);
                if(nowTime - fileTime >= m_logCycle * 24 * 60 * 60) {
                    removeList.emplace_back(file.path());
                }
            }
        }
    }
    for(auto &filePath : removeList) {
        std::filesystem::remove(filePath);
    }
}

bool PowerLogger::_createLog() {
    if(m_pFile != nullptr) {
        fclose(m_pFile);
    }
    time_t now = time(nullptr);
    tm *nowTm = localtime(&now);
    nowTm->tm_sec = 0;
    nowTm->tm_min = 0;
    nowTm->tm_hour = 0;
    now = mktime(nowTm);
    m_logCreateTime = now;
    char fileName[m_bizName.size() + 32];
    sprintf(fileName, "%s%04d%02d%02d.log",
            m_bizName.c_str(),
            nowTm->tm_year + 1900,
            nowTm->tm_mon + 1,
            nowTm->tm_mday);
    std::filesystem::path filePath = m_logDirPath / (const char *)fileName;
    m_pFile = fopen(filePath.c_str(), "a");
    if(m_pFile == nullptr) {
        perror("cannot open log file");
        return false;
    }
    return true;
}

bool PowerLogger::_isLogTimeOut() {
    time_t now = time(nullptr);
    if(now - m_logCreateTime >= 24 * 60 * 60) {
        return true;
    } else {
        return false;
    }
}
