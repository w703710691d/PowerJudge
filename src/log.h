#ifndef POWERJUDGE_LOG_H
#define POWERJUDGE_LOG_H

#include <string>

class PowerLogger {
public:
    enum LogLevel {
        FATAL = 0,
        WARNING = 1,
        MONITOR = 2,
        NOTICE = 3,
        TRACE = 4,
        DEBUG = 5
    };

    bool setLogPath(std::string path);
    void setLogFileName(std::string fileName);
    void setLogLevel(int loglevel);
    void writeLog(
        int level,
        const char* srcFilePath,
        const int srcLine,
        const char* msg, ...);
    static PowerLogger& instance();

private:
    PowerLogger();
    ~PowerLogger();

    std::string m_path;
    std::string m_fileName;
    FILE *m_pFile;
    int m_logLevel;
    const size_t LOG_BUFFER_SIZE = 8192;
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
