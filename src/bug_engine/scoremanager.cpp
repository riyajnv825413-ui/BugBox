/**
 * scoremanager.cpp
 * -----------------------------------------------------------
 * ScoreManager implementation
 * -----------------------------------------------------------
 */

#include "scoremanager.h"
#include <algorithm>  // std::max

// -----------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------
ScoreManager::ScoreManager(QObject *parent)
    : QObject(parent)
{
    // m_score initialised via in-class initialiser (= kStartingScore)
}

// -----------------------------------------------------------------------
// resetForNewRound
// Call this every time a new challenge is loaded.
// -----------------------------------------------------------------------
void ScoreManager::resetForNewSession()
{
    m_score         = kStartingScore;
    m_wrongAttempts = 0;
    m_hintUsed      = false;
    emit scoreChanged(m_score);
}

void ScoreManager::resetHintForNewChallenge()
{
    m_hintUsed = false;
}

// -----------------------------------------------------------------------
// applyWrongSubmission
// Deducts kWrongPenalty for each distinct wrong attempt.
// There is no duplicate-guard here on purpose: each press of Submit
// with a wrong answer should cost 10 points (common expectation).
// -----------------------------------------------------------------------
bool ScoreManager::applyWrongSubmission()
{
    m_wrongAttempts++;
    updateScore(-kWrongPenalty);
    return true;
}

// -----------------------------------------------------------------------
// applyHintUsed
// Only deducts once per round (hint can only be used once per challenge).
// Returns false if already deducted — the UI should ignore duplicate calls.
// -----------------------------------------------------------------------
bool ScoreManager::applyHintUsed()
{
    if (m_hintUsed) {
        return false;   // already charged — do not charge again
    }
    m_hintUsed = true;
    updateScore(-kHintPenalty);
    return true;
}

// -----------------------------------------------------------------------
// updateScore  (private helper)
// -----------------------------------------------------------------------
void ScoreManager::updateScore(int delta)
{
    m_score = std::max(0, m_score + delta);
    emit scoreChanged(m_score);
}
