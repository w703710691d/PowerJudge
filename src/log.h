#ifndef POWERJUDGE_LOG_H
#define POWERJUDGE_LOG_H

#include <string>
#include <filesystem>

class PowerLogger {
public:
    enum LogLevel {
        FATAL = 0,
        WARNING = 1,
        MONITOR = 2,
        NOTICE = 3,
        TRACE = 4,
        DEBUG = 5,
    };

    bool setLogDir(const std::string &strLogDir);
    void setBizName(const std::string &bizName);
    void setLogLevel(int loglevel);
    void setLogCycle(int days);

    void writeLog(
        int level,
        const char* srcFilePath,
        int srcLine,
        const char* msg, ...);
    static PowerLogger& instance();

private:
    PowerLogger();
    ~PowerLogger();
    std::filesystem::path m_logDirPath;
    std::string m_bizName;
    FILE *m_pFile;
    int m_logLevel;
    int m_logCycle;
    time_t m_logCreateTime;

    bool _isLogTimeOut();
    void _clearLog();
    bool _createLog();
};

#define FM_LOG_DEBUG(x...) \
PowerLogger::instance().writeLog(PowerLogger::DEBUG,   __FILE__, __LINE__, ##x)
#define FM_LOG_TRACE(x...) \
PowerLogger::instance().writeLog(PowerLogger::TRACE,   __FILE__, __LINE__, ##x)
#define FM_LOG_NOTICE(x...) \
PowerLogger::instance().writeLog(PowerLogger::NOTICE,  __FILE__, __LINE__, ##x)
#define FM_LOG_MONITOR(x...) \
PowerLogger::instance().writeLog(PowerLogger::MONITOR, __FILE__, __LINE__, ##x)
#define FM_LOG_WARNING(x...) \
PowerLogger::instance().writeLog(PowerLogger::WARNING, __FILE__, __LINE__, ##x)
#define FM_LOG_FATAL(x...)  \
PowerLogger::instance().writeLog(PowerLogger::FATAL,   __FILE__, __LINE__, ##x)

#endif //POWERJUDGE_LOG_H
