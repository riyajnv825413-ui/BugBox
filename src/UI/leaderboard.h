#ifndef LEADERBOARD_H
#define LEADERBOARD_H

// #pragma once <-- REMOVE THIS (It is redundant when you already have #ifndef guard headers above)
#include <QObject>
#include <QString>
#include <QVector>
#include <QMetaType> // <-- ADD THIS: Required for custom structs in signals/slots loops
struct LeaderboardEntry {
    QString name;
    int score = 0;
    qint64 timeMs = 0;
};

// Explicitly register the struct layout style with Qt's type pipeline system
Q_DECLARE_METATYPE(LeaderboardEntry) // <-- ADD THIS

class Leaderboard : public QObject
{
    Q_OBJECT

public:
    explicit Leaderboard(QObject *parent = nullptr);

    void setFilePath(const QString &path);
    QString filePath() const;

    void submitScore(const QString &name, int score, qint64 timeMs);
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
