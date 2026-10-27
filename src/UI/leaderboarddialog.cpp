#include <Qt> // <-- Add this to fix namespace flags
#include <QTableWidget>
#include "leaderboarddialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>

LeaderboardDialog::LeaderboardDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();

    // Initialize backend engine and connect signals
    m_leaderboardBackend = new Leaderboard(this);
    
    connect(m_leaderboardBackend, &Leaderboard::topFetched, 
            this, &LeaderboardDialog::onTopFetched);
    connect(m_leaderboardBackend, &Leaderboard::errorOccurred, 
            this, &LeaderboardDialog::onLeaderboardError);

    // Trigger the file read operation immediately on open
    m_leaderboardBackend->fetchTop10();
}

LeaderboardDialog::~LeaderboardDialog()
{
}

void LeaderboardDialog::setupUI()
{
    setObjectName("LeaderboardDialog");
    resize(500, 450);
    setMinimumSize(450, 400);
    setWindowTitle("BUGBOX - Global Leaderboard");

    // Dark/Neon stylesheet theme matching your pitch-black preferences
    setStyleSheet(R"(
        QDialog#LeaderboardDialog {
            background-color: #000000;
        }
        QLabel {
            color: #ffffff;
            font-family: 'Segoe UI', sans-serif;
        }
        QTableWidget {
            background-color: #111122;
            color: #ffffff;
            gridline-color: #333366;
            border: 1px solid #5050b0;
            border-radius: 8px;
            font-family: 'Segoe UI', sans-serif;
            font-size: 13px;
        }
        QTableWidget::item {
            padding: 5px;
        }
        QTableWidget::item:selected {
            background-color: #3a3a9a;
            color: #ffffff;
        }
        QHeaderView::section {
            background-color: #1a1a4e;
            color: #a0c4ff;
            padding: 6px;
            border: 1px solid #333366;
            font-weight: bold;
        }
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #7b2ff7, stop:1 #f107a3);
            color: #ffffff;
            border: none;
            border-radius: 6px;
            font-weight: bold;
            padding: 8px 20px;
            font-family: 'Segoe UI', sans-serif;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #9b4fff, stop:1 #ff30c0);
        }
    )");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // Title Block
    QLabel *titleLabel = new QLabel("🏆 GLOBAL LEADERBOARD 🏆", this);
    QFont titleFont("Segoe UI", 18, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

         // Table View Initialization
    m_tableWidget = new QTableWidget(this);
    m_tableWidget->setColumnCount(3);
    m_tableWidget->setHorizontalHeaderLabels({"Rank / Player", "Score", "Time"});
    
    // Direct class-based scoping to bypass namespace errors
    m_tableWidget->setEditTriggers(QTableWidget::NoEditTriggers); 
    m_tableWidget->setSelectionBehavior(QTableWidget::SelectRows);   // Scoped via QTableWidget
    m_tableWidget->setSelectionMode(QTableWidget::SingleSelection);  // Scoped via QTableWidget



    // Stretch columns neatly across dialog footprint
    m_tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_tableWidget->verticalHeader()->setVisible(false);
    
    mainLayout->addWidget(m_tableWidget);

    // Close Action Controls layout block
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    QPushButton *closeButton = new QPushButton("Close", this);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    btnLayout->addWidget(closeButton);
    
    mainLayout->addLayout(btnLayout);
    setLayout(mainLayout);
}

void LeaderboardDialog::onTopFetched(const QVector<LeaderboardEntry> &entries)
{
    m_tableWidget->setRowCount(0); // Wipe existing rows clear

    for (int i = 0; i < entries.size(); ++i) {
        m_tableWidget->insertRow(i);
        const LeaderboardEntry &entry = entries[i];

        // Format Rank prefix + Name string representation
        QString rankPrefix = QString::number(i + 1) + ". ";
        if (i == 0) rankPrefix = "🥇 ";
        else if (i == 1) rankPrefix = "🥈 ";
        else if (i == 2) rankPrefix = "🥉 ";

        QTableWidgetItem *nameItem = new QTableWidgetItem(rankPrefix + entry.name);
        QTableWidgetItem *scoreItem = new QTableWidgetItem(QString::number(entry.score));
        
        // Convert milliseconds runtime cleanly to seconds format view
        double seconds = entry.timeMs / 1000.0;
        QTableWidgetItem *timeItem = new QTableWidgetItem(QString("%1s").arg(seconds, 0, 'f', 2));

        // Align numbers rightward
        scoreItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        timeItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        m_tableWidget->setItem(i, 0, nameItem);
        m_tableWidget->setItem(i, 1, scoreItem);
        m_tableWidget->setItem(i, 2, timeItem);
    }
}

void LeaderboardDialog::onLeaderboardError(const QString &message)
{
    QMessageBox::critical(this, "Leaderboard Error", message);
}
