/**
 * bugchallengeadapter.h
 * -----------------------------------------------------------
 * BugChallengeAdapter — curated + generated challenges
 * (OOP: adapter / composition pattern)
 *
 * The curated challenges below are hand-written demo content.
 * They are combined with randomised challenges produced by the
 * teammate's BugGenerator engine (bugengineadapter.h), so the
 * GUI plays both reliable, hand-tested bugs and fresh ones.
 *
 * BugChallengeAdapter uses a QVector container (OOP: composition)
 * to store all loaded challenges and hands them out one by one.
 * -----------------------------------------------------------
 */

#ifndef BUGCHALLENGEADAPTER_H
#define BUGCHALLENGEADAPTER_H

#include <QVector>
#include "bugchallenge.h"
#include "bugengineadapter.h"

// -----------------------------------------------------------------------
// BugChallengeAdapter
// -----------------------------------------------------------------------
class BugChallengeAdapter
{
public:
    // Constructor: immediately loads mock challenges into the internal list.
    explicit BugChallengeAdapter();

    // Returns all challenges for a given difficulty level.
    QVector<BugChallenge> challengesForDifficulty(Difficulty d) const;

    // Returns the total number of loaded challenges.
    int totalCount() const { return m_challenges.size(); }

private:
    // Populates m_challenges with hard-coded sample data.
    // *** REPLACE THIS METHOD with real module calls when available. ***
    void loadMockChallenges();

    // Container holding all challenges.
    QVector<BugChallenge> m_challenges;

    // Bridges teammate-generated bugs into the GUI model.
    BugEngineAdapter m_bugEngine;
};

#endif // BUGCHALLENGEADAPTER_H
