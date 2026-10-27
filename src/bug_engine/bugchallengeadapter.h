/**
 * bugchallengeadapter.h
 * -----------------------------------------------------------
 * BugChallengeAdapter — MOCK Adapter  (OOP: adapter pattern)
 *
 * *** THIS IS A MOCK MODULE ***
 * Your teammate is responsible for the real bug-generation
 * module.  Until that module is available, this adapter
 * supplies hard-coded sample challenges so the GUI can be
 * demonstrated end-to-end.
 *
 * HOW TO REPLACE WITH THE REAL MODULE:
 *   1. Ask your teammate for their header / class interface.
 *   2. Replace the body of loadChallenges() to call their API.
 *   3. Map their data fields to BugChallenge setters.
 *
 * BugChallengeAdapter uses a QVector container (OOP: composition)
 * to store all loaded challenges and hands them out one by one.
 * -----------------------------------------------------------
 */

#ifndef BUGCHALLENGEADAPTER_H
#define BUGCHALLENGEADAPTER_H

#include <QVector>
#include "bugchallenge.h"

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
};

#endif // BUGCHALLENGEADAPTER_H
