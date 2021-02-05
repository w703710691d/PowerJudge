#include "judgetaskcontent.h"

#include <unistd.h>
#include <cstring>
#include <getopt.h>

JudgeTaskContent::JudgeTaskContent()
    : m_contestId(0),
      m_solutionId(0),
      m_problemId(0),
      m_language(ProgramLanguage::Unknown),
      m_judgeType(JudgeType::ACM),
      m_timeLimit(1000),
      m_memoryLimit(64 * STD_MB) {}

JudgeResult::JudgeResult()
    : m_result(SolutionState::Accepted), m_timeUsage(0), m_memoryUsage(0) {}



JudgeTaskContentMaker::JudgeTaskContentMaker()
{
}

JudgeTaskContentMaker::~JudgeTaskContentMaker()
{
}

JudgeTaskContentMaker& JudgeTaskContentMaker::getInstance()
{
    static JudgeTaskContentMaker s_judgeTaskContentMaker;
    return s_judgeTaskContentMaker;
}

std::shared_ptr<JudgeTaskContent> JudgeTaskContentMaker::makeJudgeTaskContentFromArgs(int argc,  char *argv[])
{
    auto judgeTaskContent = std::make_shared<JudgeTaskContent>();
    int opt;
    while ((opt = getopt(argc, argv, "s:p:t:m:l:w:d:D:j:c:")) != -1) {
        char pathBuffer[PATH_SIZE];
        switch (opt) {
            case 's':
                judgeTaskContent->m_solutionId = std::stoi(optarg);
                break;
            case 'p':
                judgeTaskContent->m_problemId = std::stoi(optarg);
                break;
            case 'l':
                judgeTaskContent->m_language =
                        static_cast<ProgramLanguage>(std::stoi(optarg));
                break;
            case 'j':
                judgeTaskContent->m_judgeType =
                        static_cast<JudgeType>(std::stoi(optarg));
                break;
            case 't':
                judgeTaskContent->m_timeLimit = std::stoull(optarg);
                break;
            case 'm':
                judgeTaskContent->m_memoryLimit = std::stoull(optarg);
            case 'w':
            case 'd':
                if (realpath(optarg, pathBuffer) == nullptr) {
                    fprintf(stderr, "resolve work dir failed:%s\n",
                            strerror(errno));
                    return nullptr;
                }
                judgeTaskContent->m_workDir = pathBuffer;
                break;
            case 'D':
                if (realpath(optarg, pathBuffer) == nullptr) {
                    fprintf(stderr, "resolve data dir failed: %s\n",
                            strerror(errno));
                    return nullptr;
                }
                judgeTaskContent->m_dataDir = pathBuffer;
                break;
            case 'c':
                judgeTaskContent->m_contestId = std::stoi(optarg);
                break;
            default:
                fprintf(stderr, "unknown option provided: -%c %s\n", opt,
                        optarg);
                return nullptr;
        }
    }

    if (judgeTaskContent->m_solutionId == 0 ||
        judgeTaskContent->m_problemId == 0 ||
        judgeTaskContent->m_dataDir.empty() ||
        judgeTaskContent->m_workDir.empty()) {
        return nullptr;
    }

    judgeTaskContent->m_workDir = judgeTaskContent->m_workDir + "/" +
                                  std::to_string(judgeTaskContent->m_solutionId);
    judgeTaskContent->m_dataDir = judgeTaskContent->m_dataDir + "/" +
                                  std::to_string(judgeTaskContent->m_problemId);

    return judgeTaskContent;
}

std::shared_ptr<JudgeTaskContent> JudgeTaskContentMaker::makeJudgeTaskContentFromJson(const std::string &json) {
    return nullptr;
}
