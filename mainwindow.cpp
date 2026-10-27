/**
 * mainwindow.cpp
 * -----------------------------------------------------------
 * MainWindow implementation — Phase 5: Navigation
 * -----------------------------------------------------------
 */

#include "mainwindow.h"

#include <QVBoxLayout>
#include <QInputDialog>
#include <QLineEdit>

#include "CompilerEngine.h"
#include "leaderboard.h"
#include "leaderboarddialog.h"

// -----------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("BUGBOX — Find the bug. Fix the code. Beat the clock.");
    setMinimumSize(1000, 680);
    resize(1200, 750);

    // BugChallengeAdapter is heap-allocated; Qt deletes it when
    // MainWindow is destroyed (parent-child memory management).
    m_adapter = new BugChallengeAdapter();

    // CompilerEngine for real compilation (teammate module).
    m_compilerEngine = new CompilerEngine();

    // Leaderboard persists scores to JSON in the user's AppData folder.
    m_leaderboard = new Leaderboard(this);
    m_playerName = qEnvironmentVariable("USERNAME");
    if (m_playerName.trimmed().isEmpty())
        m_playerName = QStringLiteral("Player");

    setupScreens();

    // Start on the Home screen.
    showScreen(kHomeIndex);
}

// -----------------------------------------------------------------------
// setupScreens  (private)
// Creates all screens, adds them to the stacked widget, and wires signals.
// -----------------------------------------------------------------------
void MainWindow::setupScreens()
{
    m_stack = new QStackedWidget(this);
    setCentralWidget(m_stack);

    // --- Home screen (index 0) ------------------------------------------
    m_home = new HomeScreen(m_stack);
    m_stack->addWidget(m_home);   // index 0

    // --- Debug screen (index 1) -----------------------------------------
    m_debug = new DebugScreen(m_stack, m_compilerEngine);
    m_stack->addWidget(m_debug);  // index 1

    // --- Result screen (index 2) ----------------------------------------
    m_result = new ResultScreen(m_stack);
    m_stack->addWidget(m_result); // index 2

    // --- Signal / slot connections (Phase 5: navigation) ----------------

    // Home → Debug
    connect(m_home,  &HomeScreen::startRequested,
            this,    &MainWindow::onStartRequested);

    // Debug → Result
    connect(m_debug, &DebugScreen::sessionFinished,
            this,    &MainWindow::onSessionFinished);

    // Debug → Home (mid-session quit)
    connect(m_debug, &DebugScreen::homeRequested,
            this,    &MainWindow::onHomeFromDebug);

    // Result → Home
    connect(m_result, &ResultScreen::homeRequested,
            this,     &MainWindow::onHomeFromResult);

    // Result → Play again (same difficulty)
    connect(m_result, &ResultScreen::playAgainRequested,
            this,     &MainWindow::onPlayAgain);

    // Home / Result → Leaderboard dialog
    connect(m_home, &HomeScreen::leaderboardRequested,
            this,   &MainWindow::onLeaderboardRequested);
    connect(m_result, &ResultScreen::leaderboardRequested,
            this,     &MainWindow::onLeaderboardRequested);
}

// -----------------------------------------------------------------------
// showScreen  (private helper)
// -----------------------------------------------------------------------
void MainWindow::showScreen(int index)
{
    m_stack->setCurrentIndex(index);
}

// -----------------------------------------------------------------------
// onStartRequested  (slot)
// Triggered by HomeScreen when user clicks "Start Debugging".
// -----------------------------------------------------------------------
void MainWindow::onStartRequested(Difficulty difficulty, GameMode mode)
{
    m_lastDifficulty = difficulty;
    m_lastGameMode   = mode;

    // Fetch challenges from the adapter for the chosen difficulty.
    QVector<BugChallenge> challenges = m_adapter->challengesForDifficulty(difficulty);

    if (mode == GameMode::QuickRound && !challenges.isEmpty()) {
        challenges = challenges.mid(0, 1);
    }

    // Hand the challenges to the debug screen and let it run.
    m_debug->startSession(challenges);
    showScreen(kDebugIndex);
}

// -----------------------------------------------------------------------
// onSessionFinished  (slot)
// Triggered by DebugScreen when all challenges are done.
// -----------------------------------------------------------------------
void MainWindow::onSessionFinished(int score, int timeSecs, int hints, int wrongs,
                                   int solved, int totalChallenges)
{
    m_result->setResults(score, timeSecs, hints, wrongs, solved, totalChallenges);

    // Persist the score if the player solved at least one challenge.
    // The name is remembered for the rest of the application session.
    if (solved > 0) {
        bool ok = false;
        const QString name = QInputDialog::getText(
            this,
            tr("Save Score"),
            tr("Name for the leaderboard:"),
            QLineEdit::Normal,
            m_playerName,
            &ok);

        if (ok && !name.trimmed().isEmpty()) {
            m_playerName = name.trimmed();
            m_leaderboard->submitScore(m_playerName, score,
                                       static_cast<qint64>(timeSecs) * 1000);
        }
    }

    showScreen(kResultIndex);
}

// -----------------------------------------------------------------------
// onLeaderboardRequested  (slot)
// Shows the teammate's JSON-backed top-10 leaderboard.
// -----------------------------------------------------------------------
void MainWindow::onLeaderboardRequested()
{
    LeaderboardDialog dialog(m_leaderboard, this);
    dialog.exec();
}

// -----------------------------------------------------------------------
// onHomeFromDebug  (slot)
// User clicked "Home" from the debug screen.
// -----------------------------------------------------------------------
void MainWindow::onHomeFromDebug()
{
    showScreen(kHomeIndex);
}

// -----------------------------------------------------------------------
// onHomeFromResult  (slot)
// -----------------------------------------------------------------------
void MainWindow::onHomeFromResult()
{
    showScreen(kHomeIndex);
}

// -----------------------------------------------------------------------
// onPlayAgain  (slot)
// Restart the same difficulty immediately.
// -----------------------------------------------------------------------
void MainWindow::onPlayAgain()
{
    onStartRequested(m_lastDifficulty, m_lastGameMode);
}
