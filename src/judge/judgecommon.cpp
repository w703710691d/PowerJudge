#include "judgecommon.h"

#include <unordered_map>

const std::string& getSolutionStateName(const SolutionState solutionState) {
    const static std::unordered_map<SolutionState, std::string>
        solutionStateMapper{
            {SolutionState::Accepted, "Accepted"},
            {SolutionState::PresentationError, "PresentationError"},
            {SolutionState::TimeLimitExceed, "TimeLimitExceed"},
            {SolutionState::MemoryLimitExceed, "MemoryLimitExceed"},
            {SolutionState::WrongAnswer, "WrongAnswer"},
            {SolutionState::RuntimeError, "RuntimeError"},
            {SolutionState::OutputLimitExceed, "OutputLimitExceed"},
            {SolutionState::CompileError, "CompileError"},
            {SolutionState::RestrictedFunction, "RestrictedFunction"},
            {SolutionState::SystemError, "SystemError"},
            {SolutionState::ValidatorError, "ValidatorError"},
            {SolutionState::Waiting, "Waiting"},
            {SolutionState::Running, "Running"},
            {SolutionState::Rejuding, "Rejuding"},
            {SolutionState::Similar, "Similar"},
            {SolutionState::Compiling, "Compiling"},
            {SolutionState::Queuing, "Queuing"},
        };
    const static std::string defualtName = "Unknow";
    auto nameFinder = solutionStateMapper.find(solutionState);
    if (nameFinder == solutionStateMapper.end()) {
        return defualtName;
    } else {
        return nameFinder->second;
    }
}

void exitProcess(ExitReson reson) { exit(static_cast<int>(reson)); }