/**
 * bugchallenge.h
 * -----------------------------------------------------------
 * BugChallenge — Plain data class (OOP: encapsulation)
 *
 * Represents a single debugging challenge.  Each challenge
 * stores the question text, buggy code snippet, expected
 * output (if any), the fixed solution, a hint, and a
 * difficulty tag.
 *
 * This class is designed so that it can later be populated
 * by your teammate's bug-generation module.  For now, the
 * BugChallengeAdapter (bugchallengeadapter.h) fills it with
 * hard-coded sample data (clearly marked as MOCK).
 * -----------------------------------------------------------
 */

#ifndef BUGCHALLENGE_H
#define BUGCHALLENGE_H

#include <QString>
#include "BugData.h"
enum class GameMode {
    FullRun,
    QuickRound  // One challenge only (good for a quick demo)
};

// -----------------------------------------------------------------------
// BugChallenge
// -----------------------------------------------------------------------
class BugChallenge
{
public:
    // --- Constructors ---------------------------------------------------

    // Default constructor — creates an empty / invalid challenge.
    BugChallenge() = default;

    // Full constructor — preferred way to create a challenge.
    BugChallenge(const QString &title,
                 const QString &description,
                 const QString &buggyCode,
                 const QString &expectedOutput,
                 const QString &solution,
                 const QString &hint,
                 Difficulty     difficulty);

    // --- Getters (read-only accessors) ----------------------------------
    QString    title()          const { return m_title;          }
    QString    description()    const { return m_description;    }
    QString    buggyCode()      const { return m_buggyCode;      }
    QString    expectedOutput() const { return m_expectedOutput; }
    QString    solution()       const { return m_solution;       }
    QString    hint()           const { return m_hint;           }
    Difficulty difficulty()     const { return m_difficulty;     }

    // --- Setters (used by the bug-generation module / adapter) ----------
    void setTitle(const QString &v)          { m_title = v;          }
    void setDescription(const QString &v)    { m_description = v;    }
    void setBuggyCode(const QString &v)      { m_buggyCode = v;      }
    void setExpectedOutput(const QString &v) { m_expectedOutput = v; }
    void setSolution(const QString &v)       { m_solution = v;       }
    void setHint(const QString &v)           { m_hint = v;           }
    void setDifficulty(Difficulty d)         { m_difficulty = d;     }

    // Returns true when the challenge has meaningful content.
    bool isValid() const { return !m_title.isEmpty() && !m_buggyCode.isEmpty(); }

private:
    QString    m_title;
    QString    m_description;
    QString    m_buggyCode;
    QString    m_expectedOutput;
    QString    m_solution;
    QString    m_hint;
    Difficulty m_difficulty { Difficulty::EASY }; // Explicitly set to uppercase EASY
};

#endif