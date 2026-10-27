/**
 * bugengineadapter.h
 * -----------------------------------------------------------
 * Adapts teammate-generated BugData into the GUI's BugChallenge
 * model (OOP: adapter pattern).
 *
 * The adapter draws randomised challenges from the bug engine
 * and converts them into the same BugChallenge objects used by
 * the rest of BUGBOX.  Validation/scoring code therefore does
 * not need to know where a challenge came from.
 * -----------------------------------------------------------
 */

#ifndef BUGENGINEADAPTER_H
#define BUGENGINEADAPTER_H

#include <QVector>

#include "bugchallenge.h"

class BugEngineAdapter
{
public:
    // Generates up to `count` distinct challenges at the requested
    // difficulty.  Duplicate templates are skipped, so callers may
    // receive fewer than `count` when the engine has no more
    // unique bugs at that level.
    QVector<BugChallenge> generateChallenges(Difficulty difficulty,
                                             int count = 2) const;

private:
    static int toEngineLevel(Difficulty difficulty);
};

#endif // BUGENGINEADAPTER_H
