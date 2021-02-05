#ifndef POWERJUDGE_COMPILERUNJUDGETASK_H
#define POWERJUDGE_COMPILERUNJUDGETASK_H
#include "judge/judgetaskcontent.h"

class CompileRunJudgeTask {
public:
    CompileRunJudgeTask(std::shared_ptr<JudgeTaskContent> content);
    virtual ~CompileRunJudgeTask();
    void start();
protected:
    virtual bool prepareEnvironment();
    virtual bool compile();
    virtual bool run();
    std::shared_ptr<JudgeTaskContent> m_content;
    std::shared_ptr<JudgeResult> m_result;
};


#endif //POWERJUDGE_COMPILERUNJUDGETASK_H
