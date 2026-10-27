/**
 * debugscreen.cpp
 * -----------------------------------------------------------
 * DebugScreen implementation — Phase 3 + 6 + 7
 *
 * VALIDATION NOTE:
 *   The Run and Submit buttons do NOT compile or execute
 *   the user's C++ code inside this process.  That would be a
 *   serious security and stability risk.  Instead, they perform
 *   a text normalisation + comparison against the stored
 *   solution (Run is a dry run, Submit records the score).
 *   Text normalisation removes C++ comments (single-line and
 *   multi-line) and collapses whitespace to make comparison
 *   robust against formatting differences.
 *
 * COMPILATION MODULE INTEGRATION (not yet available):
 *   To enable real compilation/execution, a teammate must provide
 *   a module with the following interface:
 *
 *   class CodeCompiler {
 *   public:
 *       struct Result {
 *           bool success;
 *           QString stdout;
 *           QString stderr;
 *           int exitCode;
 *       };
 *       // Compile and run userCode with a timeout (ms).
 *       // Returns Result with stdout/stderr/exitCode.
 *       virtual Result compileAndRun(const QString &userCode,
 *                                    int timeoutMs = 5000) = 0;
 *       virtual ~CodeCompiler() = default;
 *   };
 *
 *   Integration steps:
 *   1. Add the compilation module to CMakeLists.txt
 *   2. Include its header in debugscreen.h
 *   3. In DebugScreen constructor, accept a CodeCompiler* (or nullptr)
 *   4. In onRunClicked/onSubmitClicked, if compiler available:
 *         auto result = compiler->compileAndRun(userCode);
 *         Compare result.stdout with ch.expectedOutput()
 *      Else fallback to current text comparison.
 *
 *   Full sandboxed execution would require an external process
 *   (QProcess) with resource limits (CPU time, memory, no network).
 *   This is left as a clearly-labelled future enhancement.
 * -----------------------------------------------------------
 */

#include "debugscreen.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QFrame>
#include <QScrollArea>
#include <QMessageBox>
#include <QFont>
#include <QApplication>
#include <algorithm>

// -----------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------
DebugScreen::DebugScreen(QWidget *parent, CompilerEngine *compilerEngine)
    : QWidget(parent)
    , m_compilerEngine(compilerEngine)
{
    // ScoreManager is a child QObject — Qt handles its lifetime.
    m_scoreManager = new ScoreManager(this);
    connect(m_scoreManager, &ScoreManager::scoreChanged,
            this,           &DebugScreen::onScoreChanged);

    // Timer fires every 1 second.
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer, &QTimer::timeout, this, &DebugScreen::onTimerTick);

    buildLayout();
    applyStyles();
}

// -----------------------------------------------------------------------
// startSession
// Load a new list of challenges and begin at index 0.
// Calling this also stops any previously running timer cleanly.
// -----------------------------------------------------------------------
void DebugScreen::startSession(const QVector<BugChallenge> &challenges)
{
    // Stop any existing timer first (Phase 7: prevent multiple timers).
    m_timer->stop();

    m_challenges    = challenges;
    m_currentIndex  = 0;
    m_elapsedSeconds= 0;
    m_totalHints    = 0;
    m_totalWrongs   = 0;
    m_solvedCount   = 0;
    m_sessionActive = true;
    m_scoreManager->resetForNewSession();
    m_scoreLabel->setText(QString("Score: %1").arg(m_scoreManager->currentScore()));

    if (m_challenges.isEmpty()) {
        m_outputArea->setPlainText("⚠ No challenges available for this difficulty. Try another one.");
        m_sessionActive = false;
        return;
    }

    loadChallenge(0);
}

// -----------------------------------------------------------------------
// loadChallenge  (private)
// Populates all UI widgets with data from challenges[index].
// -----------------------------------------------------------------------
void DebugScreen::loadChallenge(int index)
{
    // Guard
    if (index < 0 || index >= m_challenges.size()) return;

    const BugChallenge &ch = m_challenges[index];

    // Update challenge counter label
    m_challengeNumLabel->setText(
        QString("Challenge %1 of %2").arg(index + 1).arg(m_challenges.size())
    );

    // Update title and description
    m_titleLabel->setText(ch.title());
    m_descLabel->setText(ch.description());

    // Expected output panel
    QString exp = ch.expectedOutput().isEmpty()
                      ? "(not provided)"
                      : ch.expectedOutput();
    m_expectedLabel->setText("Expected output:  " + exp);

    // Load buggy code into editor
    m_codeEditor->setPlainText(ch.buggyCode());

    // Clear output area
    m_outputArea->clear();
    m_outputArea->setPlainText("Output will appear here after you click Run or Submit.");

    // New challenge in same session — score continues; hint resets.
    m_scoreManager->resetHintForNewChallenge();
    m_hintButton->setEnabled(true);
    m_hintButton->setText("💡 Hint  (-5 pts)");
    m_nextBugButton->setVisible(false);
    m_submitButton->setEnabled(true);

    if (index == 0) {
        m_elapsedSeconds = 0;
        m_timerLabel->setText("⏱  00:00");
        m_timer->start();
    }
}

// -----------------------------------------------------------------------
// onTimerTick  (private slot)
// -----------------------------------------------------------------------
void DebugScreen::onTimerTick()
{
    m_elapsedSeconds++;
    int minutes = m_elapsedSeconds / 60;
    int seconds = m_elapsedSeconds % 60;
    m_timerLabel->setText(
        QString("⏱  %1:%2")
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'))
    );
}

// -----------------------------------------------------------------------
// onRunClicked  (private slot)
// Runs the user's code with the compiler engine if available,
// otherwise falls back to text normalisation comparison.
// -----------------------------------------------------------------------
void DebugScreen::onRunClicked()
{
    const BugChallenge &ch = m_challenges[m_currentIndex];
    QString userCode = m_codeEditor->toPlainText().trimmed();

    if (userCode.isEmpty()) {
        m_outputArea->setPlainText("⚠ The code editor is empty. Please write or edit the code first.");
        return;
    }

    // If compiler engine is available, use it for real compilation and execution.
    if (m_compilerEngine) {
        m_outputArea->setPlainText("⏳ Compiling and running...\n");
        QApplication::processEvents();

        CompilerEngine::Result result = m_compilerEngine->evaluate(
            userCode,
            QString(),  // No input for these challenges
            ch.expectedOutput(),
            2000,       // 2 second time limit
            10000       // 10 second compile limit
        );

        QString msg;
        if (result.status == CompilerEngine::Accepted) {
            msg = "✅ [Compiled & Run]  Program executed successfully!\n\n";
            msg += "Output:\n" + result.output + "\n\n";
            msg += "Expected output:\n" + ch.expectedOutput() + "\n\n";
            msg += "Time: " + QString::number(result.executionTimeMs) + " ms\n";
            msg += "Note: Click Submit to record your answer officially.";
        } else if (result.status == CompilerEngine::CompilationError) {
            msg = "❌ [Compilation Error]\n\n";
            msg += result.errorOutput.isEmpty() ? result.message : result.errorOutput;
            msg += "\n\nFix the errors and try again.";
        } else if (result.status == CompilerEngine::RuntimeError) {
            msg = "⚠ [Runtime Error]\n\n";
            msg += result.message + "\n\n";
            msg += "Output:\n" + result.output + "\n";
            msg += "Error:\n" + result.errorOutput;
        } else if (result.status == CompilerEngine::TimeLimitExceeded) {
            msg = "⏱ [Time Limit Exceeded]\n\n";
            msg += "The program took longer than 2 seconds to execute.\n";
            msg += "Time: " + QString::number(result.executionTimeMs) + " ms";
        } else {
            msg = "❌ [" + CompilerEngine::statusName(result.status) + "]\n\n";
            msg += result.message;
        }

        m_outputArea->setPlainText(msg);
        return;
    }

    // Fallback: textual dry-run comparison.
    bool looksFixed = (normalise(userCode) == normalise(ch.solution()));

    if (looksFixed) {
        m_outputArea->setPlainText(
            "✅ [Dry Run]  Your code matches the solution (ignoring comments & whitespace)!\n\n"
            "Expected output:\n" + ch.expectedOutput() + "\n\n"
            "Note: Click Submit to record your answer officially."
        );
    } else {
        m_outputArea->setPlainText(
            "🔍 [Dry Run]  Your code differs from the solution (ignoring comments & whitespace).\n\n"
            "Expected output:\n" + (ch.expectedOutput().isEmpty() ? "(none)" : ch.expectedOutput()) + "\n\n"
            "Note: This is a text comparison, not actual compilation.\n"
            "Hint: Focus on fixing the logic, not the formatting."
        );
    }
}

// -----------------------------------------------------------------------
// onHintClicked  (private slot)
// -----------------------------------------------------------------------
void DebugScreen::onHintClicked()
{
    const BugChallenge &ch = m_challenges[m_currentIndex];

    if (!m_scoreManager->applyHintUsed()) {
        return;   // already used this challenge — no second −5
    }
    m_totalHints++;
    m_hintButton->setEnabled(false);
    m_hintButton->setText("💡 Hint (used)");

    m_outputArea->setPlainText(
        "💡 Hint:\n\n" + ch.hint() + "\n\n"
        "(Hint costs 5 points — you can only use one hint per challenge.)"
    );
}

// -----------------------------------------------------------------------
// onSubmitClicked  (private slot)
// Validates user code using compiler engine if available,
// otherwise falls back to text normalisation (ignores comments & whitespace).
// Updates score once per submission.
// -----------------------------------------------------------------------
void DebugScreen::onSubmitClicked()
{
    if (!m_sessionActive) return;

    const BugChallenge &ch = m_challenges[m_currentIndex];
    QString userCode = m_codeEditor->toPlainText().trimmed();

    if (userCode.isEmpty()) {
        m_outputArea->setPlainText("⚠ Nothing to submit — the editor is empty.");
        return;
    }

    bool correct = false;

    // If compiler engine is available, use it for real compilation and execution.
    if (m_compilerEngine) {
        m_outputArea->setPlainText("⏳ Compiling and running...\n");
        QApplication::processEvents();

        CompilerEngine::Result result = m_compilerEngine->evaluate(
            userCode,
            QString(),  // No input for these challenges
            ch.expectedOutput(),
            2000,       // 2 second time limit
            10000       // 10 second compile limit
        );

        correct = (result.status == CompilerEngine::Accepted);

        if (correct) {
            m_outputArea->setPlainText(
                "✅ [Compiled & Run]  Program executed successfully!\n\n"
                "Output:\n" + result.output + "\n\n"
                "Expected output:\n" + ch.expectedOutput() + "\n\n"
                "Time: " + QString::number(result.executionTimeMs) + " ms"
            );
        } else {
            QString msg;
            if (result.status == CompilerEngine::CompilationError) {
                msg = "❌ [Compilation Error]\n\n";
                msg += result.errorOutput.isEmpty() ? result.message : result.errorOutput;
            } else if (result.status == CompilerEngine::RuntimeError) {
                msg = "⚠ [Runtime Error]\n\n";
                msg += result.message + "\n\n";
                msg += "Output:\n" + result.output + "\n";
                msg += "Error:\n" + result.errorOutput;
            } else if (result.status == CompilerEngine::TimeLimitExceeded) {
                msg = "⏱ [Time Limit Exceeded]\n\n";
                msg += "The program took longer than 2 seconds to execute.\n";
                msg += "Time: " + QString::number(result.executionTimeMs) + " ms";
            } else {
                msg = "❌ [" + CompilerEngine::statusName(result.status) + "]\n\n";
                msg += result.message;
            }
            msg += "\n\nFix the errors and try again.";
            m_outputArea->setPlainText(msg);
        }
    } else {
        // Fallback: textual comparison with normalisation.
        correct = (normalise(userCode) == normalise(ch.solution()));

        if (correct) {
            m_outputArea->setPlainText(
                "✅ Correct!  Well done!\n\n"
                "Your code matches the solution (ignoring comments & whitespace)."
            );
        } else {
            m_outputArea->setPlainText(
                "❌ Incorrect.  Keep trying!\n\n"
                "Your code does not match the expected fix (ignoring comments & whitespace)."
            );
        }
    }

    if (correct) {
        m_solvedCount++;
        m_submitButton->setEnabled(false);

        int mins = m_elapsedSeconds / 60;
        int secs = m_elapsedSeconds % 60;

        m_outputArea->appendPlainText(
            QString("\nScore this round: %1\n"
                    "Time taken: %2:%3\n\n"
                    "Click 'Next Bug →' to continue.")
                .arg(m_scoreManager->currentScore())
                .arg(mins, 2, 10, QChar('0'))
                .arg(secs, 2, 10, QChar('0'))
        );

        // Show "Next Bug" or finish.
        if (m_currentIndex + 1 < m_challenges.size()) {
            m_nextBugButton->setVisible(true);
            m_nextBugButton->setText("Next Bug →");
        } else {
            m_nextBugButton->setVisible(true);
            m_nextBugButton->setText("🏁 Finish");
        }

    } else {
        m_scoreManager->applyWrongSubmission();
        m_totalWrongs++;

        m_outputArea->appendPlainText(
            QString("\n-10 points.  Current score: %1\n\n"
                    "Tip: Re-read the description and look at the expected output.")
                .arg(m_scoreManager->currentScore())
        );
    }
}

// -----------------------------------------------------------------------
// onNextBugClicked  (private slot)
// -----------------------------------------------------------------------
void DebugScreen::onNextBugClicked()
{
    m_currentIndex++;

    if (m_currentIndex < m_challenges.size()) {
        // Reset hint button text for new challenge
        m_hintButton->setText("💡 Hint  (-5 pts)");
        loadChallenge(m_currentIndex);
    } else {
        endSession();
    }
}

// -----------------------------------------------------------------------
// onHomeClicked  (private slot)
// -----------------------------------------------------------------------
void DebugScreen::onHomeClicked()
{
    m_timer->stop();
    m_sessionActive = false;
    emit homeRequested();
}

// -----------------------------------------------------------------------
// onScoreChanged  (private slot)
// -----------------------------------------------------------------------
void DebugScreen::onScoreChanged(int newScore)
{
    m_scoreLabel->setText(QString("Score: %1").arg(newScore));
}

// -----------------------------------------------------------------------
// endSession  (private)
// -----------------------------------------------------------------------
void DebugScreen::endSession()
{
    m_timer->stop();
    m_sessionActive = false;
    emit sessionFinished(
        m_scoreManager->currentScore(),
        m_elapsedSeconds,
        m_totalHints,
        m_totalWrongs,
        m_solvedCount,
        m_challenges.size()
    );
}

// -----------------------------------------------------------------------
// normalise  (private helper)
// Strips C++ comments and normalises whitespace to make code comparison
// robust against formatting differences, comments, indentation, etc.
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
// normalise  (private helper)
// Strips C++ comments and normalises whitespace to make code comparison
// robust against formatting differences, comments, indentation, etc.
// -----------------------------------------------------------------------
QString DebugScreen::normalise(const QString &s) const
{
    QString result = s;
    result.remove('\r');

    // Remove single-line comments (// ...) - simple state machine
    QString noSingleLineComments;
    bool inComment = false;
    for (int i = 0; i < result.length(); ++i) {
        QChar c = result[i];
        if (!inComment && c == '/' && i + 1 < result.length() && result[i + 1] == '/') {
            inComment = true;
            ++i; // skip the second '/'
        } else if (inComment && c == '\n') {
            inComment = false;
            noSingleLineComments.append(c);
        } else if (!inComment) {
            noSingleLineComments.append(c);
        }
    }

    // Remove multi-line comments (/* ... */) - simple state machine
    QString noComments;
    inComment = false;
    for (int i = 0; i < noSingleLineComments.length(); ++i) {
        QChar c = noSingleLineComments[i];
        if (!inComment && c == '/' && i + 1 < noSingleLineComments.length() && noSingleLineComments[i + 1] == '*') {
            inComment = true;
            ++i; // skip the '*'
        } else if (inComment && c == '*' && i + 1 < noSingleLineComments.length() && noSingleLineComments[i + 1] == '/') {
            inComment = false;
            ++i; // skip the '/'
        } else if (!inComment) {
            noComments.append(c);
        }
    }

    // Collapse all runs of whitespace to a single space
    noComments = noComments.simplified();
    return noComments;
}

// -----------------------------------------------------------------------
// buildLayout  (private)
// -----------------------------------------------------------------------
void DebugScreen::buildLayout()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ====== TOP BAR =====================================================
    auto *topBar = new QWidget(this);
    topBar->setObjectName("topBar");
    auto *topLayout = new QHBoxLayout(topBar);
    topLayout->setContentsMargins(16, 10, 16, 10);

    m_homeButton = new QPushButton("🏠 Home", topBar);
    m_homeButton->setObjectName("homeButton");
    m_homeButton->setCursor(Qt::PointingHandCursor);
    connect(m_homeButton, &QPushButton::clicked, this, &DebugScreen::onHomeClicked);
    topLayout->addWidget(m_homeButton);

    m_challengeNumLabel = new QLabel("Challenge 0 of 0", topBar);
    m_challengeNumLabel->setObjectName("challengeNumLabel");
    topLayout->addWidget(m_challengeNumLabel, 0, Qt::AlignCenter);

    topLayout->addStretch();

    m_timerLabel = new QLabel("⏱  00:00", topBar);
    m_timerLabel->setObjectName("timerLabel");
    topLayout->addWidget(m_timerLabel);

    topLayout->addSpacing(20);

    m_scoreLabel = new QLabel("Score: 100", topBar);
    m_scoreLabel->setObjectName("scoreLabel");
    topLayout->addWidget(m_scoreLabel);

    mainLayout->addWidget(topBar);

    // ====== MAIN CONTENT (splitter) =====================================
    auto *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setObjectName("mainSplitter");
    splitter->setHandleWidth(4);
    mainLayout->addWidget(splitter, 1);

    // ------ LEFT PANEL: description + output ----------------------------
    auto *leftPanel = new QWidget(splitter);
    leftPanel->setObjectName("leftPanel");
    auto *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(16, 16, 8, 16);
    leftLayout->setSpacing(12);

    m_titleLabel = new QLabel("Challenge Title", leftPanel);
    m_titleLabel->setObjectName("challengeTitle");
    m_titleLabel->setWordWrap(true);
    leftLayout->addWidget(m_titleLabel);

    auto *descScroll = new QScrollArea(leftPanel);
    descScroll->setWidgetResizable(true);
    descScroll->setObjectName("descScroll");
    descScroll->setFrameShape(QFrame::NoFrame);

    m_descLabel = new QLabel(leftPanel);
    m_descLabel->setObjectName("descLabel");
    m_descLabel->setWordWrap(true);
    m_descLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_descLabel->setMargin(8);
    descScroll->setWidget(m_descLabel);
    leftLayout->addWidget(descScroll, 1);

    m_expectedLabel = new QLabel("Expected output: —", leftPanel);
    m_expectedLabel->setObjectName("expectedLabel");
    m_expectedLabel->setWordWrap(true);
    leftLayout->addWidget(m_expectedLabel);

    auto *outTitle = new QLabel("Output / Feedback:", leftPanel);
    outTitle->setObjectName("outTitle");
    leftLayout->addWidget(outTitle);

    m_outputArea = new QPlainTextEdit(leftPanel);
    m_outputArea->setObjectName("outputArea");
    m_outputArea->setReadOnly(true);
    m_outputArea->setPlainText("Output will appear here after you click Run or Submit.");
    m_outputArea->setMaximumHeight(160);
    leftLayout->addWidget(m_outputArea);

    splitter->addWidget(leftPanel);

    // ------ RIGHT PANEL: code editor + buttons --------------------------
    auto *rightPanel = new QWidget(splitter);
    rightPanel->setObjectName("rightPanel");
    auto *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(8, 16, 16, 16);
    rightLayout->setSpacing(10);

    auto *editorTitle = new QLabel("C++ Code Editor:", rightPanel);
    editorTitle->setObjectName("editorTitle");
    rightLayout->addWidget(editorTitle);

    m_codeEditor = new QPlainTextEdit(rightPanel);
    m_codeEditor->setObjectName("codeEditor");
    m_codeEditor->setTabStopDistance(32);  // 4-space-equivalent tab
    QFont monoFont("Courier New", 12);
    monoFont.setFixedPitch(true);
    m_codeEditor->setFont(monoFont);
    rightLayout->addWidget(m_codeEditor, 1);

    // ---- Buttons row ---------------------------------------------------
    auto *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(12);
    btnLayout->setContentsMargins(0, 8, 0, 0);

    m_runButton = new QPushButton("▶  Run", rightPanel);
    m_runButton->setObjectName("runButton");
    m_runButton->setCursor(Qt::PointingHandCursor);
    m_runButton->setToolTip(
        "Dry-run check: compares your code (ignoring comments/whitespace)\n"
        "against the stored solution. Does NOT compile or execute C++."
    );
    m_runButton->setMinimumHeight(44);
    m_runButton->setMinimumWidth(110);
    connect(m_runButton, &QPushButton::clicked, this, &DebugScreen::onRunClicked);
    btnLayout->addWidget(m_runButton);

    m_hintButton = new QPushButton("💡 Hint  (-5 pts)", rightPanel);
    m_hintButton->setObjectName("hintButton");
    m_hintButton->setCursor(Qt::PointingHandCursor);
    m_hintButton->setMinimumHeight(44);
    m_hintButton->setMinimumWidth(130);
    connect(m_hintButton, &QPushButton::clicked, this, &DebugScreen::onHintClicked);
    btnLayout->addWidget(m_hintButton);

    m_submitButton = new QPushButton("✔  Submit", rightPanel);
    m_submitButton->setObjectName("submitButton");
    m_submitButton->setCursor(Qt::PointingHandCursor);
    m_submitButton->setMinimumHeight(44);
    m_submitButton->setMinimumWidth(120);
    connect(m_submitButton, &QPushButton::clicked, this, &DebugScreen::onSubmitClicked);
    btnLayout->addWidget(m_submitButton);

    m_nextBugButton = new QPushButton("Next Bug →", rightPanel);
    m_nextBugButton->setObjectName("nextBugButton");
    m_nextBugButton->setCursor(Qt::PointingHandCursor);
    m_nextBugButton->setVisible(false);
    m_nextBugButton->setMinimumHeight(44);
    m_nextBugButton->setMinimumWidth(130);
    connect(m_nextBugButton, &QPushButton::clicked, this, &DebugScreen::onNextBugClicked);
    btnLayout->addWidget(m_nextBugButton);

    // Wrap buttons in a container widget to prevent collapse
    auto *btnContainer = new QWidget(rightPanel);
    btnContainer->setObjectName("btnContainer");
    btnContainer->setLayout(btnLayout);
    btnContainer->setMinimumHeight(60);

    rightLayout->addWidget(btnContainer);
    splitter->addWidget(rightPanel);

    // Set equal initial split
    splitter->setSizes({400, 600});
}

// -----------------------------------------------------------------------
// applyStyles  (private)
// -----------------------------------------------------------------------
void DebugScreen::applyStyles()
{
    setStyleSheet(R"(
        /* Background */
        DebugScreen {
            background: #0d0d1a;
        }

        /* Top bar */
        QWidget#topBar {
            background: #13132b;
            border-bottom: 1px solid #2a2a5a;
        }

        QLabel#challengeNumLabel {
            font-size: 13px;
            color: #8080c0;
            font-family: 'Segoe UI', sans-serif;
        }

        QLabel#timerLabel {
            font-size: 15px;
            font-weight: bold;
            color: #ff9f40;
            font-family: 'Segoe UI', sans-serif;
        }

        QLabel#scoreLabel {
            font-size: 15px;
            font-weight: bold;
            color: #7bff7b;
            font-family: 'Segoe UI', sans-serif;
        }

        /* Panels */
        QWidget#leftPanel, QWidget#rightPanel {
            background: #0d0d1a;
        }

        /* Challenge title */
        QLabel#challengeTitle {
            font-size: 20px;
            font-weight: bold;
            color: #ffffff;
            font-family: 'Segoe UI', sans-serif;
        }

        /* Description scroll area */
        QScrollArea#descScroll {
            background: transparent;
        }

        /* Description text */
        QLabel#descLabel {
            font-size: 13px;
            color: #c8d0ff;
            background: rgba(255,255,255,0.04);
            border: 1px solid #2a2a5a;
            border-radius: 8px;
            padding: 12px;
            font-family: 'Segoe UI', sans-serif;
        }

        /* Expected output */
        QLabel#expectedLabel {
            font-size: 13px;
            color: #90e890;
            background: rgba(0,255,100,0.04);
            border: 1px solid #1a5a1a;
            border-radius: 6px;
            padding: 8px 12px;
            font-family: 'Courier New', monospace;
        }

        /* Output area */
        QLabel#outTitle, QLabel#editorTitle {
            font-size: 12px;
            color: #7080a0;
            font-family: 'Segoe UI', sans-serif;
        }

        QPlainTextEdit#outputArea {
            background: #0a0a18;
            color: #a0f0a0;
            border: 1px solid #1a3a1a;
            border-radius: 6px;
            font-family: 'Courier New', monospace;
            font-size: 12px;
            padding: 8px;
        }

        /* Code editor */
        QPlainTextEdit#codeEditor {
            background: #1a1a2e;
            color: #e0e0ff;
            border: 1px solid #3030a0;
            border-radius: 8px;
            font-family: 'Courier New', monospace;
            font-size: 13px;
            padding: 10px;
            selection-background-color: #3a3a8a;
        }

        /* Splitter handle */
        QSplitter#mainSplitter::handle {
            background: #2a2a5a;
        }

        /* Buttons */
        QPushButton#runButton {
            background: #1a5a1a;
            color: #90ff90;
            border: 1px solid #2a8a2a;
            border-radius: 8px;
            padding: 8px 18px;
            font-size: 13px;
            font-weight: bold;
        }
        QPushButton#runButton:hover { background: #2a7a2a; }

        QPushButton#hintButton {
            background: #5a4a00;
            color: #ffd060;
            border: 1px solid #8a7000;
            border-radius: 8px;
            padding: 8px 18px;
            font-size: 13px;
            font-weight: bold;
        }
        QPushButton#hintButton:hover { background: #7a6a00; }
        QPushButton#hintButton:disabled { color: #554400; border-color: #443300; background: #2a2000; }

        QPushButton#submitButton {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 #7b2ff7, stop:1 #f107a3);
            color: #ffffff;
            border: none;
            border-radius: 8px;
            padding: 8px 18px;
            font-size: 13px;
            font-weight: bold;
        }
        QPushButton#submitButton:hover {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 #9b4fff, stop:1 #ff30c0);
        }
        QPushButton#submitButton:disabled { background: #333; color: #666; }

        QPushButton#nextBugButton {
            background: #1a3a6a;
            color: #80c0ff;
            border: 1px solid #3060aa;
            border-radius: 8px;
            padding: 8px 18px;
            font-size: 13px;
            font-weight: bold;
        }
        QPushButton#nextBugButton:hover { background: #2a5a9a; }

        QPushButton#homeButton {
            background: rgba(255,255,255,0.06);
            color: #a0a0d0;
            border: 1px solid #3a3a6a;
            border-radius: 8px;
            padding: 6px 14px;
            font-size: 12px;
        }
        QPushButton#homeButton:hover { background: rgba(255,255,255,0.12); }
    )");
}
