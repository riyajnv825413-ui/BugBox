/**
 * bugchallenge.cpp
 * -----------------------------------------------------------
 * Implementation of the BugChallenge full constructor.
 * -----------------------------------------------------------
 */

#include "bugchallenge.h"

BugChallenge::BugChallenge(const QString &title,
                           const QString &description,
                           const QString &buggyCode,
                           const QString &expectedOutput,
                           const QString &solution,
                           const QString &hint,
                           Difficulty     difficulty)
    : m_title(title)
    , m_description(description)
    , m_buggyCode(buggyCode)
    , m_expectedOutput(expectedOutput)
    , m_solution(solution)
    , m_hint(hint)
    , m_difficulty(difficulty)
{
}
