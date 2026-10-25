/**
 * scoremanager.h
 * -----------------------------------------------------------
 * ScoreManager — Manages score for one round  (OOP: SRP class)
 *
 * Rules:
 *   Starting score  : 100
 *   Wrong submission: -10 (guarded against double-deduction)
 *   Hint used       : -5  (guarded against double-deduction)
 *   Score floor     : 0   (never goes negative)
 *
 * Score carries across challenges in one session. Only the hint
 * flag resets when a new challenge loads.
 * -----------------------------------------------------------
 */

#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include <QObject>

// -----------------------------------------------------------------------
// ScoreManager
// -----------------------------------------------------------------------
class ScoreManager : public QObject
{
    Q_OBJECT

public:
    // Constants
    static constexpr int kStartingScore     = 100;
    static constexpr int kWrongPenalty      = 10;
    static constexpr int kHintPenalty       = 5;

    explicit ScoreManager(QObject *parent = nullptr);

    // --- Round lifecycle ------------------------------------------------

    // Resets score to 100 at the start of a new game session.
    void resetForNewSession();

    // Resets hint usage only (new challenge in the same session).
    void resetHintForNewChallenge();

    // --- Deductions (each is idempotent for the same event) -------------

    // Deduct for a wrong submission.
    // Returns false if the deduction was already applied this attempt,
    // so the caller can decide whether to show feedback.
    bool applyWrongSubmission();

    // Deduct for using a hint.
    // Returns false if the hint was already used this round.
    bool applyHintUsed();

    // --- Accessors ------------------------------------------------------
    int  currentScore()       const { return m_score;           }
    int  wrongAttempts()      const { return m_wrongAttempts;   }
    bool hintUsedThisRound()  const { return m_hintUsed;        }

signals:
    // Emitted whenever the score changes so the UI can update instantly.
    void scoreChanged(int newScore);

private:
    int  m_score          { kStartingScore };
    int  m_wrongAttempts  { 0 };
    bool m_hintUsed       { false };

    // Clamps m_score to [0, kStartingScore] and emits scoreChanged.
    void updateScore(int delta);
};

#endif // SCOREMANAGER_H
