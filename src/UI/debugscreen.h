/**
 * debugscreen.h
 * -----------------------------------------------------------
 * DebugScreen — Phase 3: The main debugging / gameplay screen
 *
 * Contains:
 *   - Challenge title and description (QLabel)
 *   - C++ code editor          (QPlainTextEdit)
 *   - Expected output panel    (QLabel, read-only)
 *   - Output / feedback area   (QPlainTextEdit, read-only)
 *   - Timer display            (QLabel + QTimer)
 *   - Score display            (QLabel)
 *   - Run, Hint, Submit, Next Bug buttons
 *
 * Inherits QWidget (OOP: inheritance).
 * Owns a ScoreManager (OOP: composition / aggregation).
 * Optionally uses a CompilerEngine for real compilation (teammate module).
 * -----------------------------------------------------------
 */

#ifndef DEBUGSCREEN_H
#define DEBUGSCREEN_H

#include <QWidget>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTimer>
#include <QVector>

#include "bugchallenge.h"
#include "scoremanager.h"
#include "CompilerEngine.h"

// -----------------------------------------------------------------------
// DebugScreen
// -----------------------------------------------------------------------
class DebugScreen : public QWidget
{
    Q_OBJECT

public:
    // Constructor with optional CompilerEngine for real compilation.
    // If compilerEngine is null, falls back to text comparison.
    explicit DebugScreen(QWidget *parent = nullptr,
                         CompilerEngine *compilerEngine = nullptr);
    ~DebugScreen() override = default;

    // Load a list of challenges and start the first one.
    // Called by MainWindow every time the Debug screen is shown.
    void startSession(const QVector<BugChallenge> &challenges);

signals:
    // Emitted when the user finishes (solved all challenges or gives up).
    void sessionFinished(int finalScore, int timeTaken, int hintsUsed, int wrongs,
                         int solvedCount, int totalChallenges);

    // Emitted when the user wants to go back Home mid-session.
    void homeRequested();

private slots:
    void onTimerTick();
    void onRunClicked();
    void onHintClicked();
    void onSubmitClicked();
    void onNextBugClicked();
    void onHomeClicked();
    void onScoreChanged(int newScore);

private:
    // --- Helpers --------------------------------------------------------
    void buildLayout();
    void applyStyles();
    void loadChallenge(int index);
    void endSession();
    QString normalise(const QString &s) const;

    // --- Session state --------------------------------------------------
    QVector<BugChallenge> m_challenges;
    int   m_currentIndex  { 0 };
    int   m_elapsedSeconds{ 0 };
    int   m_totalHints    { 0 };
    int   m_totalWrongs   { 0 };
    int   m_solvedCount   { 0 };
    bool  m_sessionActive { false };

    // --- Core objects ---------------------------------------------------
    ScoreManager    *m_scoreManager { nullptr };
    QTimer          *m_timer        { nullptr };
    CompilerEngine  *m_compilerEngine { nullptr };

    // --- UI Widgets -----------------------------------------------------
    QLabel         *m_challengeNumLabel  { nullptr };
    QLabel         *m_titleLabel         { nullptr };
    QLabel         *m_descLabel          { nullptr };
    QLabel         *m_expectedLabel      { nullptr };
    QPlainTextEdit *m_codeEditor         { nullptr };
    QPlainTextEdit *m_outputArea         { nullptr };
    QLabel         *m_timerLabel         { nullptr };
    QLabel         *m_scoreLabel         { nullptr };
    QPushButton    *m_runButton          { nullptr };
    QPushButton    *m_hintButton         { nullptr };
    QPushButton    *m_submitButton       { nullptr };
    QPushButton    *m_nextBugButton      { nullptr };
    QPushButton    *m_homeButton         { nullptr };
};

#endif // DEBUGSCREEN_H
