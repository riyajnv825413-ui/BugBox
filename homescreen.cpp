/**
 * homescreen.cpp
 * -----------------------------------------------------------
 * HomeScreen implementation — polished home/welcome screen
 * -----------------------------------------------------------
 */

#include "homescreen.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>

// -----------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------
HomeScreen::HomeScreen(QWidget *parent)
    : QWidget(parent)
{
    buildLayout();
    applyStyles();
}

// -----------------------------------------------------------------------
// selectedDifficulty
// Maps combo-box index to the Difficulty enum.
// -----------------------------------------------------------------------
Difficulty HomeScreen::selectedDifficulty() const
{
    switch (m_difficultyCombo->currentIndex()) {
    case 1:  return Difficulty::Medium;
    case 2:  return Difficulty::Hard;
    default: return Difficulty::Easy;
    }
}

GameMode HomeScreen::selectedGameMode() const
{
    return (m_gameModeCombo->currentIndex() == 1)
               ? GameMode::QuickRound
               : GameMode::FullRun;
}

// -----------------------------------------------------------------------
// onStartClicked  (private slot)
// -----------------------------------------------------------------------
void HomeScreen::onStartClicked()
{
    emit startRequested(selectedDifficulty(), selectedGameMode());
}

void HomeScreen::onLeaderboardClicked()
{
    emit leaderboardRequested();
}

// -----------------------------------------------------------------------
// buildLayout  (private)
// Creates all child widgets and arranges them with Qt layouts.
// -----------------------------------------------------------------------
void HomeScreen::buildLayout()
{
    // ---- Main vertical layout ------------------------------------------
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setSpacing(18);
    mainLayout->setContentsMargins(60, 60, 60, 60);

    // ---- Title ---------------------------------------------------------
    m_titleLabel = new QLabel("🐛 BUGBOX", this);
    m_titleLabel->setObjectName("titleLabel");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_titleLabel);

    // ---- Subtitle ------------------------------------------------------
    m_subtitleLabel = new QLabel("Find the bug.  Fix the code.  Beat the clock.", this);
    m_subtitleLabel->setObjectName("subtitleLabel");
    m_subtitleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_subtitleLabel);

    mainLayout->addSpacing(10);

    // ---- Horizontal separator ------------------------------------------
    auto *sep = new QFrame(this);
    sep->setFrameShape(QFrame::HLine);
    sep->setObjectName("separator");
    mainLayout->addWidget(sep);

    mainLayout->addSpacing(10);

    // ---- Instructions card --------------------------------------------
    m_instructLabel = new QLabel(
        "<b>How to Play</b><br><br>"
        "1. Choose difficulty and game mode below.<br>"
        "2. Read the challenge description carefully.<br>"
        "3. Find and fix the bug in the C++ code editor.<br>"
        "4. Press <b>Submit</b> to check your answer.<br>"
        "5. Use <b>Hint</b> if you are stuck (costs 5 points).<br>"
        "6. Each wrong submission costs 10 points.<br>"
        "7. Beat the clock for bonus bragging rights! ⏱",
        this
    );
    m_instructLabel->setObjectName("instructLabel");
    m_instructLabel->setWordWrap(true);
    m_instructLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->addWidget(m_instructLabel);

    mainLayout->addSpacing(20);

    // ---- Difficulty selector -------------------------------------------
    auto *diffRow = new QHBoxLayout();
    diffRow->setAlignment(Qt::AlignCenter);

    auto *diffLabel = new QLabel("Difficulty:", this);
    diffLabel->setObjectName("diffLabel");
    diffRow->addWidget(diffLabel);

    m_difficultyCombo = new QComboBox(this);
    m_difficultyCombo->setObjectName("difficultyCombo");
    m_difficultyCombo->addItem("🟢  Easy");
    m_difficultyCombo->addItem("🟡  Medium");
    m_difficultyCombo->addItem("🔴  Hard");
    m_difficultyCombo->setMinimumWidth(180);
    diffRow->addWidget(m_difficultyCombo);

    mainLayout->addLayout(diffRow);

    // ---- Game mode selector --------------------------------------------
    auto *modeRow = new QHBoxLayout();
    modeRow->setAlignment(Qt::AlignCenter);

    auto *modeLabel = new QLabel("Game Mode:", this);
    modeLabel->setObjectName("diffLabel");
    modeRow->addWidget(modeLabel);

    m_gameModeCombo = new QComboBox(this);
    m_gameModeCombo->setObjectName("gameModeCombo");
    m_gameModeCombo->addItem("📋  Full Run  (all bugs at this level)");
    m_gameModeCombo->addItem("⚡  Quick Round  (one challenge)");
    m_gameModeCombo->setMinimumWidth(320);
    modeRow->addWidget(m_gameModeCombo);

    mainLayout->addLayout(modeRow);

    mainLayout->addSpacing(10);

    // ---- Start button --------------------------------------------------
    m_startButton = new QPushButton("▶  Start Debugging", this);
    m_startButton->setObjectName("startButton");
    m_startButton->setMinimumHeight(50);
    m_startButton->setMinimumWidth(240);
    m_startButton->setCursor(Qt::PointingHandCursor);
    connect(m_startButton, &QPushButton::clicked, this, &HomeScreen::onStartClicked);

    m_leaderboardButton = new QPushButton("🏆  Leaderboard", this);
    m_leaderboardButton->setObjectName("leaderboardButton");
    m_leaderboardButton->setMinimumHeight(50);
    m_leaderboardButton->setMinimumWidth(180);
    m_leaderboardButton->setCursor(Qt::PointingHandCursor);
    connect(m_leaderboardButton, &QPushButton::clicked,
            this, &HomeScreen::onLeaderboardClicked);

    auto *btnRow = new QHBoxLayout();
    btnRow->setAlignment(Qt::AlignCenter);
    btnRow->setSpacing(12);
    btnRow->addWidget(m_startButton);
    btnRow->addWidget(m_leaderboardButton);
    mainLayout->addLayout(btnRow);

    mainLayout->addStretch();

    // ---- Footer --------------------------------------------------------
    auto *footerLabel = new QLabel("BUGBOX  |  College Project Demo", this);
    footerLabel->setObjectName("footerLabel");
    footerLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(footerLabel);
}

// -----------------------------------------------------------------------
// applyStyles  (private)
// All styling via Qt stylesheet — avoids touching the .ui file.
// -----------------------------------------------------------------------
void HomeScreen::applyStyles()
{
    setStyleSheet(R"(
        /* ---- Background ---- */
        HomeScreen {
            background: qlineargradient(
                x1:0, y1:0, x2:0, y2:1,
                stop:0 #0f0c29, stop:0.5 #1a1a4e, stop:1 #0f0c29
            );
        }

        /* ---- Title ---- */
        QLabel#titleLabel {
            font-size: 56px;
            font-weight: 900;
            color: #ffffff;
            letter-spacing: 6px;
            font-family: 'Segoe UI', 'Arial', sans-serif;
        }

        /* ---- Subtitle ---- */
        QLabel#subtitleLabel {
            font-size: 18px;
            color: #a0c4ff;
            font-style: italic;
            font-family: 'Segoe UI', sans-serif;
        }

        /* ---- Separator ---- */
        QFrame#separator {
            color: #3a3a7a;
            max-height: 1px;
        }

        /* ---- Instruction box ---- */
        QLabel#instructLabel {
            font-size: 14px;
            color: #d0d8ff;
            background: rgba(255,255,255,0.05);
            border: 1px solid rgba(160,196,255,0.2);
            border-radius: 12px;
            padding: 20px 24px;
            line-height: 1.6;
            font-family: 'Segoe UI', sans-serif;
        }

        /* ---- Difficulty label ---- */
        QLabel#diffLabel {
            font-size: 15px;
            color: #a0c4ff;
            font-family: 'Segoe UI', sans-serif;
        }

        /* ---- Combo box ---- */
        QComboBox#difficultyCombo, QComboBox#gameModeCombo {
            background: rgba(255,255,255,0.08);
            color: #ffffff;
            border: 1px solid #5050b0;
            border-radius: 8px;
            padding: 8px 14px;
            font-size: 14px;
            font-family: 'Segoe UI', sans-serif;
        }
        QComboBox#difficultyCombo::drop-down,
        QComboBox#gameModeCombo::drop-down {
            border: none;
        }
        QComboBox#difficultyCombo QAbstractItemView,
        QComboBox#gameModeCombo QAbstractItemView {
            background: #1a1a4e;
            color: #ffffff;
            selection-background-color: #3a3a9a;
        }

        /* ---- Start button ---- */
        QPushButton#startButton {
            background: qlineargradient(
                x1:0, y1:0, x2:1, y2:0,
                stop:0 #7b2ff7, stop:1 #f107a3
            );
            color: #ffffff;
            border: none;
            border-radius: 10px;
            font-size: 17px;
            font-weight: bold;
            padding: 12px 32px;
            font-family: 'Segoe UI', sans-serif;
        }
        QPushButton#startButton:hover {
            background: qlineargradient(
                x1:0, y1:0, x2:1, y2:0,
                stop:0 #9b4fff, stop:1 #ff30c0
            );
        }
        QPushButton#startButton:pressed {
            background: qlineargradient(
                x1:0, y1:0, x2:1, y2:0,
                stop:0 #5b10d7, stop:1 #d00080
            );
        }

        /* ---- Leaderboard button ---- */
        QPushButton#leaderboardButton {
            background: rgba(255,255,255,0.08);
            color: #a0c4ff;
            border: 1px solid #3a3a8a;
            border-radius: 10px;
            font-size: 15px;
            font-weight: bold;
            padding: 12px 24px;
            font-family: 'Segoe UI', sans-serif;
        }
        QPushButton#leaderboardButton:hover {
            background: rgba(255,255,255,0.16);
            color: #ffffff;
        }

        /* ---- Footer ---- */
        QLabel#footerLabel {
            font-size: 11px;
            color: #555580;
            font-family: 'Segoe UI', sans-serif;
        }
    )");
}
