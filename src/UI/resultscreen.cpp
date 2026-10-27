/**
 * resultscreen.cpp
 * -----------------------------------------------------------
 * ResultScreen implementation — Phase 4 (Leaderboard Added)
 * -----------------------------------------------------------
 */

#include "resultscreen.h"
#include "leaderboarddialog.h" // <-- Added to display the window

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>

// -----------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------
ResultScreen::ResultScreen(QWidget *parent)
    : QWidget(parent)
{
    buildLayout();
    applyStyles();
}

// -----------------------------------------------------------------------
// setResults
// Populates all display labels before the screen is shown.
// -----------------------------------------------------------------------
void ResultScreen::setResults(const QString &playerName,int score, int timeTakenSeconds,
                              int hintsUsed, int wrongCount,
                              int solvedCount, int totalChallenges)
{
    // Score banner
    if (score >= 80) {
        m_bannerLabel->setText("🏆  Excellent Work!");
    } else if (score >= 50) {
        m_bannerLabel->setText("👍  Good Job!");
    } else if (score >= 20) {
        m_bannerLabel->setText("🙂  Keep Practising!");
    } else {
        m_bannerLabel->setText("💪  Never Give Up!");
    }

    m_scoreLabel->setText(QString("Final Score: %1 / 100").arg(score));

    m_solvedLabel->setText(
        QString("Challenges Solved: %1 / %2")
            .arg(solvedCount)
            .arg(totalChallenges)
    );

    int mins = timeTakenSeconds / 60;
    int secs = timeTakenSeconds % 60;
    m_timeLabel->setText(
        QString("Time Taken: %1 min %2 sec")
            .arg(mins)
            .arg(secs, 2, 10, QChar('0'))
    );

    m_hintsLabel->setText(
        QString("Hints Used: %1  (−%2 pts total)").arg(hintsUsed).arg(hintsUsed * 5)
    );

    m_wrongsLabel->setText(
        QString("Wrong Submissions: %1  (−%2 pts total)").arg(wrongCount).arg(wrongCount * 10)
    );

    // Rank badge
    if (score == 100) {
        m_rankLabel->setText("⭐ Perfect Score — Bug Slayer!");
    } else if (score >= 80) {
        m_rankLabel->setText("🥇 Rank: Debug Master");
    } else if (score >= 60) {
        m_rankLabel->setText("🥈 Rank: Code Inspector");
    } else if (score >= 40) {
        m_rankLabel->setText("🥉 Rank: Bug Hunter");
    } else {
        m_rankLabel->setText("🔰 Rank: Apprentice Debugger");
    }
    // Save these variables locally for the submission engine to access safely
    m_storedPlayerName = playerName;
    m_storedScore = score;
    m_storedTimeSeconds = timeTakenSeconds;
}

// -----------------------------------------------------------------------
// Slots
// -----------------------------------------------------------------------
void ResultScreen::onPlayAgainClicked() { emit playAgainRequested(); }
void ResultScreen::onHomeClicked()      { emit homeRequested(); }

void ResultScreen::onLeaderboardClicked()
{
    // 1. Instantiate the background data ledger and save the round data
    Leaderboard backend(this);
    
    // Pass name, score, and compute total milliseconds running runtime
    qint64 timeMs = static_cast<qint64>(m_storedTimeSeconds) * 1000;
    
    backend.submitScore(m_storedPlayerName, m_storedScore, timeMs);

    // Instantiates the dialog on the heap safely so it draws properly
    LeaderboardDialog *dialog = new LeaderboardDialog(this);
    dialog->setModal(true);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
    dialog->raise();
    dialog->activateWindow();
}

// -----------------------------------------------------------------------
// buildLayout  (private)
// -----------------------------------------------------------------------
void ResultScreen::buildLayout()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(80, 60, 80, 60);

    // Banner
    m_bannerLabel = new QLabel("🏆  Great Work!", this);
    m_bannerLabel->setObjectName("bannerLabel");
    m_bannerLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_bannerLabel);

    // Separator
    auto *sep = new QFrame(this);
    sep->setFrameShape(QFrame::HLine);
    sep->setObjectName("separator");
    mainLayout->addWidget(sep);

    // Stats card
    auto *card = new QWidget(this);
    card->setObjectName("statsCard");
    auto *cardLayout = new QVBoxLayout(card);
    cardLayout->setSpacing(14);
    cardLayout->setContentsMargins(30, 24, 30, 24);

    m_scoreLabel  = new QLabel("Final Score: 0 / 100", card);
    m_scoreLabel->setObjectName("resultScoreLabel");
    m_scoreLabel->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(m_scoreLabel);

    m_solvedLabel = new QLabel("Challenges Solved: 0 / 0", card);
    m_solvedLabel->setObjectName("resultStatLabel");
    m_solvedLabel->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(m_solvedLabel);

    m_timeLabel   = new QLabel("Time Taken: 0 min 00 sec", card);
    m_timeLabel->setObjectName("resultStatLabel");
    m_timeLabel->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(m_timeLabel);

    m_hintsLabel  = new QLabel("Hints Used: 0  (−0 pts total)", card);
    m_hintsLabel->setObjectName("resultStatLabel");
    m_hintsLabel->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(m_hintsLabel);

    m_wrongsLabel = new QLabel("Wrong Submissions: 0  (−0 pts total)", card);
    m_wrongsLabel->setObjectName("resultStatLabel");
    m_wrongsLabel->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(m_wrongsLabel);

    mainLayout->addWidget(card);
    

    // Rank badge
    m_rankLabel = new QLabel("🥇 Rank: Debug Master", this);
    m_rankLabel->setObjectName("rankLabel");
    m_rankLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_rankLabel);

    mainLayout->addSpacing(10);

    // Buttons Row Layout
    auto *btnRow = new QHBoxLayout();
    btnRow->setAlignment(Qt::AlignCenter);
    btnRow->setSpacing(20);

    m_homeBtn = new QPushButton("🏠  Home", this);
    m_homeBtn->setObjectName("homeBtn");
    m_homeBtn->setMinimumHeight(46);
    m_homeBtn->setMinimumWidth(140);
    m_homeBtn->setCursor(Qt::PointingHandCursor);
    connect(m_homeBtn, &QPushButton::clicked, this, &ResultScreen::onHomeClicked);
    btnRow->addWidget(m_homeBtn);

    // New Leaderboard Button added in the middle slot
    m_leaderboardBtn = new QPushButton("🏆  Leaderboard", this);
    m_leaderboardBtn->setObjectName("leaderboardBtn");
    m_leaderboardBtn->setMinimumHeight(46);
    m_leaderboardBtn->setMinimumWidth(150);
    m_leaderboardBtn->setCursor(Qt::PointingHandCursor);
    connect(m_leaderboardBtn, &QPushButton::clicked, this, &ResultScreen::onLeaderboardClicked);
    btnRow->addWidget(m_leaderboardBtn);

    m_playAgainBtn = new QPushButton("▶  Play Again", this);
    m_playAgainBtn->setObjectName("playAgainBtn");
    m_playAgainBtn->setMinimumHeight(46);
    m_playAgainBtn->setMinimumWidth(140);
    m_playAgainBtn->setCursor(Qt::PointingHandCursor);
    connect(m_playAgainBtn, &QPushButton::clicked, this, &ResultScreen::onPlayAgainClicked);
    btnRow->addWidget(m_playAgainBtn);

    mainLayout->addLayout(btnRow);
    mainLayout->addStretch();
}

// -----------------------------------------------------------------------
// applyStyles  (private)
// -----------------------------------------------------------------------
void ResultScreen::applyStyles()
{
    setStyleSheet(R"(
        ResultScreen {
            background-color: #000000; /* Overridden to match your true black theme */
        }

        QLabel#bannerLabel {
            font-size: 46px;
            font-weight: 900;
            color: #ffffff;
            font-family: 'Segoe UI', sans-serif;
            background: transparent;
        }

        QFrame#separator {
            color: #3a3a7a;
            max-height: 1px;
        }

        QWidget#statsCard {
            background: rgba(255,255,255,0.06);
            border: 1px solid rgba(160,196,255,0.2);
            border-radius: 14px;
        }

        QLabel#resultScoreLabel {
            font-size: 28px;
            font-weight: bold;
            color: #ffd060;
            font-family: 'Segoe UI', sans-serif;
            background: transparent;
        }

        QLabel#resultStatLabel {
            font-size: 15px;
            color: #c0c8ff;
            font-family: 'Segoe UI', sans-serif;
            background: transparent;
        }

        QLabel#rankLabel {
            font-size: 20px;
            font-weight: bold;
            color: #a0c4ff;
            background: rgba(100,100,255,0.1);
            border: 1px solid rgba(100,100,255,0.3);
            border-radius: 10px;
            padding: 10px 24px;
            font-family: 'Segoe UI', sans-serif;
        }

        QPushButton#homeBtn {
            background: rgba(255,255,255,0.08);
            color: #a0c4ff;
            border: 1px solid #3a3a8a;
            border-radius: 10px;
            font-size: 15px;
            font-weight: bold;
            padding: 10px 24px;
        }
        QPushButton#homeBtn:hover { background: rgba(255,255,255,0.15); }

        QPushButton#leaderboardBtn {
            background: rgba(255,255,255,0.08);
            color: #ffffff;
            border: 1px solid #7b2ff7;
            border-radius: 10px;
            font-size: 15px;
            font-weight: bold;
            padding: 10px 24px;
        }
        QPushButton#leaderboardBtn:hover { background: rgba(123, 47, 247, 0.25); }

        QPushButton#playAgainBtn {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #7b2ff7, stop:1 #f107a3
            );
            color: #ffffff;
            border: none;
            border-radius: 10px;
            font-size: 15px;
            font-weight: bold;
            padding: 10px 24px;
        }
        QPushButton#playAgainBtn:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #9b4fff, stop:1 #ff30c0
            );
        }
    )");
}
