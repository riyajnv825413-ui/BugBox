/**
 * bugenginebridge.h
 * -----------------------------------------------------------
 * Small bridge between the teammate's standalone BugGenerator
 * engine and the Qt application.
 *
 * WHY THIS EXISTS:
 *   The bug engine defines a global `Difficulty` enum and uses
 *   std::string.  The GUI also defines a `Difficulty` enum in
 *   bugchallenge.h.  Including both in one translation unit
 *   would clash.  This bridge is the ONLY file that includes the
 *   bug engine; it converts the result to Qt types and an int
 *   difficulty code.  The adapter then includes this bridge plus
 *   the GUI's BugChallenge model, with no clash.
 * -----------------------------------------------------------
 */

#ifndef BUGENGINEBRIDGE_H
#define BUGENGINEBRIDGE_H

#include <QString>

// Difficulty codes used by the bridge (avoid a second enum clash).
namespace BugEngineLevel {
    constexpr int Easy   = 0;
    constexpr int Medium = 1;
    constexpr int Hard   = 2;
}

struct GeneratedBugData
{
    QString bugType;
    int     difficulty { BugEngineLevel::Easy };
    QString buggyCode;
    QString expectedOutput;
    QString hint;
    QString solution;
};

// Generates one random bug at the requested level.
// Returns an empty struct if the engine is unavailable.
GeneratedBugData generateBugFromEngine(int difficultyLevel);

// True when the engine produced at least one valid bug.
bool bugEngineIsAvailable();

#endif // BUGENGINEBRIDGE_H
