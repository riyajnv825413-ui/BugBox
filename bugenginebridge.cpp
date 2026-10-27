/**
 * bugenginebridge.cpp
 * -----------------------------------------------------------
 * Implementation of the Qt <-> BugGenerator bridge.
 * -----------------------------------------------------------
 */

#include "bugenginebridge.h"

#include "bug_engine/BugGenerator.h"

#include <string>

// Convert std::string to QString without assuming a code page.
static QString toQt(const std::string &text)
{
    return QString::fromStdString(text);
}

GeneratedBugData generateBugFromEngine(int difficultyLevel)
{
    GeneratedBugData result;

    Difficulty engineDifficulty = Difficulty::EASY;

    switch (difficultyLevel)
    {
    case BugEngineLevel::Medium: engineDifficulty = Difficulty::MEDIUM; break;
    case BugEngineLevel::Hard:   engineDifficulty = Difficulty::HARD;   break;
    default:                     engineDifficulty = Difficulty::EASY;   break;
    }

    BugGenerator generator;
    const BugData bug = generator.generateBug(engineDifficulty);

    result.bugType        = toQt(bug.bugType);
    result.buggyCode      = toQt(bug.buggyCode);
    result.expectedOutput = toQt(bug.expectedOutput);
    result.hint           = toQt(bug.hint);
    result.solution       = toQt(bug.solution);
    result.difficulty     = difficultyLevel;

    return result;
}

bool bugEngineIsAvailable()
{
    // Generate one easy bug as a smoke test.  A valid template
    // always has non-empty code and expected output.
    const GeneratedBugData bug = generateBugFromEngine(BugEngineLevel::Easy);

    return !bug.buggyCode.isEmpty() && !bug.solution.isEmpty();
}
