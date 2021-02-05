#include "judgetaskbase.h"

CompileRunJudgeTask::CompileRunJudgeTask(std::shared_ptr<JudgeTaskContent> content) {
    m_content = content;
    m_result = std::make_shared<JudgeResult>();
}

CompileRunJudgeTask::~CompileRunJudgeTask() {

}

void CompileRunJudgeTask::start() {
    if(!prepareEnvironment()) {
        return;
    }
}

