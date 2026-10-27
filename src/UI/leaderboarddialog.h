#ifndef LEADERBOARDDIALOG_H
#define LEADERBOARDDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include "leaderboard.h"


class LeaderboardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LeaderboardDialog(QWidget *parent = nullptr);
    ~LeaderboardDialog();

private slots:
    // Slot that captures the vector from the backend and populates the table
    void onTopFetched(const QVector<LeaderboardEntry> &entries);
    void onLeaderboardError(const QString &message);

private:
    void setupUI();

    QTableWidget *m_tableWidget;
    Leaderboard *m_leaderboardBackend;
};

#endif // LEADERBOARDDIALOG_H
