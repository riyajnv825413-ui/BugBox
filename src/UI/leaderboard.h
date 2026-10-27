#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <QObject>
#include <QString>
#include <QVector>

struct LeaderboardEntry {
    QString name;
    int score = 0;
    qint64 timeMs = 0;
};

// Leaderboard stored in a local JSON file (leaderboard.json).
// The interface is the same as before: call submitScore() after a round,
// call fetchTop10() and listen to topFetched() to show the table.
class Leaderboard : public QObject
{
    Q_OBJECT

public:
    explicit Leaderboard(QObject *parent = nullptr);

    // Optional: change where the JSON file is saved.
    void setFilePath(const QString &path);
    QString filePath() const;

    // Adds one score to the file.
    void submitScore(const QString &name, int score, qint64 timeMs);

    // Reads the file and emits topFetched() with the best 10
    // (highest score first; on a tie, the faster time wins).
    void fetchTop10();

signals:
    void scoreSubmitted();
    void topFetched(const QVector<LeaderboardEntry> &entries);
    void errorOccurred(const QString &message);

private:
    QVector<LeaderboardEntry> loadAll() const;
    bool saveAll(const QVector<LeaderboardEntry> &entries) const;

    QString m_filePath;
};

#endif // LEADERBOARD_H
