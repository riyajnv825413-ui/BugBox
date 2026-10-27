/**
 * bugengineadapter.cpp
 * -----------------------------------------------------------
 * Converts generated BugData records into BugChallenge objects.
 * -----------------------------------------------------------
 */

#include "bugengineadapter.h"
#include "bugenginebridge.h"

#include <QSet>
#include <QString>

int BugEngineAdapter::toEngineLevel(Difficulty difficulty)
{
    switch (difficulty)
    {
    case Difficulty::Medium: return BugEngineLevel::Medium;
    case Difficulty::Hard:   return BugEngineLevel::Hard;
    default:                 return BugEngineLevel::Easy;
    }
}

QVector<BugChallenge> BugEngineAdapter::generateChallenges(Difficulty difficulty,
                                                           int count) const
{
    QVector<BugChallenge> challenges;

    if (count <= 0)
        return challenges;

    // The engine picks a template at random, so retry until we
    // have `count` distinct bugs (or give up after a sane limit).
    QSet<QString> seenCode;
    const int maxAttempts = count * 25 + 25;

    for (int attempt = 0; attempt < maxAttempts && challenges.size() < count;
         ++attempt)
    {
        const GeneratedBugData bug = generateBugFromEngine(toEngineLevel(difficulty));

        if (bug.buggyCode.isEmpty() || bug.solution.isEmpty())
            continue;

        if (seenCode.contains(bug.buggyCode))
            continue;

        seenCode.insert(bug.buggyCode);

        BugChallenge challenge;
        challenge.setTitle(QStringLiteral("Generated: %1").arg(bug.bugType));
        challenge.setDescription(
            QStringLiteral("Bug Engine challenge — %1.\n\n"
                           "Find and fix the bug so the program prints exactly:\n%2")
                .arg(bug.bugType, bug.expectedOutput));
        challenge.setBuggyCode(bug.buggyCode);
        challenge.setExpectedOutput(bug.expectedOutput);
        challenge.setSolution(bug.solution);
        challenge.setHint(bug.hint);
        challenge.setDifficulty(difficulty);

        if (challenge.isValid())
            challenges.append(challenge);
    }

    return challenges;
}
