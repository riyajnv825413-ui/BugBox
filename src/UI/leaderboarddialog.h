#ifndef LEADERBOARDDIALOG_H
#define LEADERBOARDDIALOG_H

#include "leaderboard.h"

#include <QDialog>

class QLabel;
class QTableWidget;

// A window showing the top 10 scores in a table.
class LeaderboardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LeaderboardDialog(Leaderboard *board, QWidget *parent = nullptr);

public slots:
    void refresh();

private slots:
    void onTopFetched(const QVector<LeaderboardEntry> &entries);
    void onError(const QString &message);

private:
    Leaderboard *m_board;
    QTableWidget *m_table;
    QLabel *m_status;
};

#endif // LEADERBOARDDIALOG_H
