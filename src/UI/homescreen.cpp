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
#include "leaderboarddialog.h"


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
    case 1:  return Difficulty::MEDIUM;
    case 2:  return Difficulty::HARD;
    default: return Difficulty::EASY;
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
    QString playerName = m_nameLineEdit->text().trimmed();
    
    if (playerName.isEmpty()) {
        // Fallback default name if left blank so it doesn't break
        playerName = "Player_1"; 
    }

    // Capture selections
    QString difficulty = m_difficultyCombo->currentText();
    QString gameMode = m_gameModeCombo->currentText();
emit startRequested(m_nameLineEdit->text().trimmed().isEmpty() ? "Player_1" : m_nameLineEdit->text().trimmed(), 
                    selectedDifficulty(), 
                    selectedGameMode());
}
void HomeScreen::onLeaderboardButtonClicked()
{
    // Fix: Create the window on the Heap using a pointer so it survives the function scope
    LeaderboardDialog *dialog = new LeaderboardDialog(this);
    
    // Set explicit visual properties to prevent 0x0 footprint sizes
    dialog->setModal(true);
    dialog->setAttribute(Qt::WA_DeleteOnClose); // Tells Qt to automatically free memory when closed
    
    // Open the window
    dialog->show(); 
    dialog->raise();
    dialog->activateWindow();
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

    // Inside buildLayout() right before the difficulty label/combo code:
m_nameLabel = new QLabel("Enter Player Name:", this);
m_nameLabel->setObjectName("nameLabel");

m_nameLineEdit = new QLineEdit(this);
m_nameLineEdit->setObjectName("nameLineEdit");
m_nameLineEdit->setPlaceholderText("Enter your username...");
m_nameLineEdit->setMaxLength(20);

// Add them to your main layout structure
mainLayout->addWidget(m_nameLabel);
mainLayout->addWidget(m_nameLineEdit);


    // ---- Start button --------------------------------------------------
    m_startButton = new QPushButton("▶  Start Debugging", this);
    m_startButton->setObjectName("startButton");
    m_startButton->setMinimumHeight(50);
    m_startButton->setMinimumWidth(240);
    m_startButton->setCursor(Qt::PointingHandCursor);
    connect(m_startButton, &QPushButton::clicked, this, &HomeScreen::onStartClicked);

    auto *btnRow = new QHBoxLayout();
    btnRow->setAlignment(Qt::AlignCenter);
    btnRow->addWidget(m_startButton);
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
    // Important: Targeted 'HomeScreen, QWidget' ensures custom layout backgrounds trigger correctly
    setStyleSheet(R"(
        /* ---- Background ---- */
        HomeScreen, QWidget {
            background-color: #000000;
        }

        /* ---- Title ---- */
        QLabel#titleLabel {
            font-size: 56px;
            font-weight: 900;
            color: #ffffff; /* Flipped to white so it pops on black background */
            letter-spacing: 6px;
            font-family: 'Segoe UI', 'Arial', sans-serif;
            background: transparent;
        }

        /* ---- Subtitle ---- */
        QLabel#subtitleLabel {
            font-size: 18px;
            color: #a0c4ff; /* Flipped to neon blue for visibility */
            font-style: italic;
            font-family: 'Segoe UI', sans-serif;
            background: transparent;
        }

        /* ---- Separator ---- */
        QFrame#separator {
            color: #3a3a7a;
            max-height: 1px;
        }

        /* ---- Instruction box ---- */
        QLabel#instructLabel {
            font-size: 14px;
            color: #e0e0ff; /* Changed to light tint */
            background: rgba(255,255,255,0.05);
            border: 1px solid rgba(160,196,255,0.2);
            border-radius: 12px;
            padding: 20px 24px;
            line-height: 1.6;
            font-family: 'Segoe UI', sans-serif;
        }

        /* ---- Difficulty label ---- */
        QLabel#diffLabel, QLabel {
            font-size: 15px;
            color: #ffffff; /* Flipped to white */
            font-family: 'Segoe UI', sans-serif;
            background: transparent;
        }

        /* ---- Combo box ---- */
        QComboBox#difficultyCombo, QComboBox#gameModeCombo, QComboBox {
            background: rgba(255,255,255,0.12); /* Slightly opaque box over black */
            color: #ffffff; /* Text color inside boxes turned to white */
            border: 1px solid #5050b0;
            border-radius: 8px;
            padding: 8px 14px;
            font-size: 14px;
            font-family: 'Segoe UI', sans-serif;
        }
        
        QComboBox::drop-down {
            border: none;
        }
        
        /* Popup dropdown panel list configuration */
        QComboBox QAbstractItemView {
            background-color: #1a1a4e;
            color: #ffffff;
            selection-background-color: #3a3a9a;
            selection-color: #ffffff;
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

        /* ---- Footer ---- */
        QLabel#footerLabel {
            font-size: 11px;
            color: #7575a3;
            font-family: 'Segoe UI', sans-serif;
            background: transparent;
        }
            /* Add this inside the raw stylesheet string string block */
QLabel#nameLabel {
    font-size: 15px;
    color: #ffffff;
    font-family: 'Segoe UI', sans-serif;
    margin-top: 10px;
}

QLineEdit#nameLineEdit {
    background: rgba(255,255,255,0.08);
    color: #ffffff;
    border: 1px solid #5050b0;
    border-radius: 8px;
    padding: 8px 14px;
    font-size: 14px;
    font-family: 'Segoe UI', sans-serif;
}
QLineEdit#nameLineEdit:focus {
    border: 1px solid #7b2ff7;
}

    )");
}
