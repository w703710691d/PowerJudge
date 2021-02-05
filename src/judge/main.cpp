#include "judgecommon.h"
#include "judgetaskcontent.h"

int main(int argc, char *argv[]) {
    auto judgeTaskContent =
            JudgeTaskContentMaker::getInstance().makeJudgeTaskContentFromArgs(argc, argv);
    if(judgeTaskContent) {
        exitProcess(ExitReason::BAD_PARAM);
    }
}