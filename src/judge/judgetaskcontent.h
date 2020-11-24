#ifndef __JUDGE_TASK_CONTENT_H__
#define __JUDGE_TASK_CONTENT_H__

#include <string>

#include "judgecommon.h"

struct JudgeTaskContent {
    JudgeTaskContent();

    int m_contestId;
    int m_solutionId;
    int m_problemId;
    ProgramLanguage m_language;
    JudgeType m_judgeType;
    size_t m_timeLimit;    // ms
    size_t m_memoryLimit;  // KB
    std::string m_workDir;
    std::string m_dataDir;
};

struct JudgeResult {
    JudgeResult();
    SolutionState m_result;
    size_t m_timeUsage;    // ms
    size_t m_memoryUsage;  // KB
};

ExitReson buildJudgeTaskContent(JudgeTaskContent &judgeTaskContent, int argc,
                                char *argv[]);

#endif