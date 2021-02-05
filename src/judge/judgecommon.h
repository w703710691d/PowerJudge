#ifndef __JUDGE_COMMON_H__
#define __JUDGE_COMMON_H__

#include <string>

//测评结果
enum class SolutionState {
    Accepted = 0,
    PresentationError = 1,
    TimeLimitExceed = 2,
    MemoryLimitExceed = 3,
    WrongAnswer = 4,
    RuntimeError = 5,
    OutputLimitExceed = 6,
    CompileError = 7,
    RestrictedFunction = 8,
    SystemError = 9,
    ValidatorError = 10,
    Waiting = 11,
    Running = 12,
    Rejudging = 13,
    Similar = 14,
    Compiling = 15,
    Queuing = 16,
};
//根据测评结果获取对应的字符串
const std::string &getSolutionStateName(const SolutionState solutionState);

const size_t STD_KB = 1ULL << 10U;
const size_t STD_MB = 1ULL << 20U;
const size_t PATH_SIZE = 4096ULL;
const size_t BUFF_SIZE = 8192ULL;

//测评类型
enum class JudgeType {
    ACM = 0,
    OI = 1,
    TopCoder = 2,
    CodeForces = 3,
};

// SPJ测评结果
enum class SpecialJudgeResult {
    Accepted = 0,
    PresentationError = 1,
    WrongAnswer = 2,
};

//退出原因
enum class ExitReason {
    OK = 0,
    UNPRIVILEGED = 1,
    CHDIR = 2,
    BAD_PARAM = 3,
    MISS_PARAM = 4,
    VERY_FIRST = 5,
    FORK_COMPILER = 6,
    COMPILE_IO = 7,
    COMPILE_EXEC = 8,
    COMPILE_ERROR = 9,
    NO_SOURCE_CODE = 10,
    PRIVILEGED = 11,
    PRE_JUDGE = 12,
    PRE_JUDGE_PTRACE = 13,
    PRE_JUDGE_EXECLP = 14,
    PRE_JUDGE_DAA = 15,
    FORK_ERROR = 16,
    EXEC_ERROR = 17,
    SET_LIMIT = 18,
    CURL_ERROR = 19,
    SET_SECURITY = 20,
    JUDGE = 21,
    COMPARE = 27,
    COMPARE_SPJ = 30,
    COMPARE_SPJ_FORK = 31,
    TIMEOUT = 36,
    UNKNOWN = 127,
};

void exitProcess(ExitReason reason);

//错误原因
enum class ErrorReason {
    GCC_COMPILE_ERROR = 1,
    ERROR_READ_FILE = 40,
    ERROR_READ_RESULT = 41,
};

//程序语言
enum class ProgramLanguage {
    Unknown = 0,
    C11 = 1,
    Cpp11 = 2,
    Pascal = 3,
    Java = 4,
    Python27 = 5,
    C99 = 6,
    Cpp98 = 7,
    Cpp14 = 8,
    Cpp17 = 9,
    Python3 = 10,
    Kotlin = 11,
    Cpp20 = 12,
    C18 = 13,
};

#endif