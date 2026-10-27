#include "leaderboarddialog.h"

#include <QAbstractItemView>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

LeaderboardDialog::LeaderboardDialog(Leaderboard *board, QWidget *parent)
    : QDialog(parent)
    , m_board(board)
{
    setWindowTitle(tr("Leaderboard"));
    resize(480, 380);

    m_status = new QLabel(tr("Loading..."), this);

    m_table = new QTableWidget(0, 4, this);
    m_table->setHorizontalHeaderLabels({tr("Rank"), tr("Name"), tr("Score"), tr("Time (s)")});
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionMode(QAbstractItemView::NoSelection);
    m_table->verticalHeader()->setVisible(false);
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    auto *refreshButton = new QPushButton(tr("Refresh"), this);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_table);
    layout->addWidget(m_status);
    layout->addWidget(refreshButton);

    connect(refreshButton, &QPushButton::clicked, this, &LeaderboardDialog::refresh);
    connect(m_board, &Leaderboard::topFetched, this, &LeaderboardDialog::onTopFetched);
    connect(m_board, &Leaderboard::errorOccurred, this, &LeaderboardDialog::onError);

    refresh();
}

void LeaderboardDialog::refresh()
{
    m_status->setText(tr("Loading..."));
    m_board->fetchTop10();
}

void LeaderboardDialog::onTopFetched(const QVector<LeaderboardEntry> &entries)
{
    m_table->setRowCount(entries.size());

    for (int i = 0; i < entries.size(); ++i) {
        const LeaderboardEntry &e = entries.at(i);
        m_table->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        m_table->setItem(i, 1, new QTableWidgetItem(e.name));
        m_table->setItem(i, 2, new QTableWidgetItem(QString::number(e.score)));
        m_table->setItem(i, 3, new QTableWidgetItem(QString::number(e.timeMs / 1000.0, 'f', 1)));
    }

    m_status->setText(entries.isEmpty() ? tr("No scores yet. Be the first!") : QString());
}

void LeaderboardDialog::onError(const QString &message)
{
    m_status->setText(message);
}
