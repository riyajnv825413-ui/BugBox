#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include "CompilerEngine.h"

// -----------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Initialize the generated Qt UI layout
    ui->setupUi(this);

    setWindowTitle("BUGBOX — Find the bug. Fix the code. Beat the clock.");
    setMinimumSize(1000, 680);
    resize(1200, 750);

    // BugChallengeAdapter is heap-allocated; Qt deletes it when
    // MainWindow is destroyed (parent-child memory management).
    m_adapter = new BugChallengeAdapter();

    // CompilerEngine for real compilation (teammate module).
    m_compilerEngine = new CompilerEngine();

    setupScreens();
    setStyleSheet(R"(
        /* ---- Force global window backgrounds to pure black ---- */
        QMainWindow, QStackedWidget, HomeScreen, DebugScreen, ResultScreen {
            background-color: #000000;
        }

        /* ---- Central Card Containers ---- */
        QWidget#statsCard, QWidget#centerContainer {
            background: rgba(255, 255, 255, 0.05);
            border: 1px solid rgba(160, 196, 255, 0.2);
            border-radius: 14px;
        }

        /* ---- Universally make labels legible white text ---- */
        QLabel {
            color: #ffffff;
            font-family: 'Segoe UI', sans-serif;
            background: transparent;
        }
        
        /* Highlight specific sub-captions/hints */
        QLabel#subtitleLabel, QLabel#resultStatLabel {
            color: #a0c4ff;
        }
        
        /* Title adjustments */
        QLabel#titleLabel, QLabel#bannerLabel {
            font-size: 46px;
            font-weight: 900;
            color: #ffffff;
            letter-spacing: 4px;
        }

        /* ---- Dropdown ComboBoxes & Inputs ---- */
        QComboBox, QLineEdit {
            background: rgba(255, 255, 255, 0.08);
            color: #ffffff;
            border: 1px solid #5050b0;
            border-radius: 8px;
            padding: 8px 14px;
            font-size: 14px;
        }
        QLineEdit:focus, QComboBox:focus {
            border: 1px solid #7b2ff7;
        }

        /* Dropdown selection view panels */
        QComboBox QAbstractItemView {
            background-color: #1a1a4e;
            color: #ffffff;
            selection-background-color: #3a3a9a;
            selection-color: #ffffff;
            border: 1px solid #5050b0;
        }

        /* ---- Buttons ---- */
        QPushButton {
            background: rgba(255, 255, 255, 0.08);
            color: #a0c4ff;
            border: 1px solid #3a3a8a;
            border-radius: 10px;
            font-size: 15px;
            font-weight: bold;
            padding: 10px 24px;
        }
        QPushButton:hover { 
            background: rgba(255, 255, 255, 0.15); 
        }

        /* Standout action button overrides (like Start Debugging) */
        QPushButton#m_startBtn, QPushButton#startButton, QPushButton#playAgainBtn {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #7b2ff7, stop:1 #f107a3
            );
            color: #ffffff;
            border: none;
        }
        QPushButton#m_startBtn:hover, QPushButton#startButton:hover, QPushButton#playAgainBtn:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #9b4fff, stop:1 #ff30c0
            );
        }
    )");

    // Start on the Home screen.
    showScreen(kHomeIndex);
   
}

// -----------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
    // Note: m_adapter, m_compilerEngine, and m_stack are automatically 
    // deleted by Qt because they are bound to 'this' as children.
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
void MainWindow::onStartRequested(const QString &playerName,Difficulty difficulty, GameMode mode)
{
    m_playerName     = playerName;
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
    m_result->setResults(m_playerName,score, timeSecs, hints, wrongs, solved, totalChallenges);
    showScreen(kResultIndex);
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
    onStartRequested(m_playerName,m_lastDifficulty, m_lastGameMode);
}


    