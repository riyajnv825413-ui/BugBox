#include "leaderboard.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

#include <algorithm>

static void sortEntries(QVector<LeaderboardEntry> &entries)
{
    std::sort(entries.begin(), entries.end(),
              [](const LeaderboardEntry &a, const LeaderboardEntry &b) {
                  if (a.score != b.score)
                      return a.score > b.score;   // higher score first
                  return a.timeMs < b.timeMs;     // then faster time
              });
}

Leaderboard::Leaderboard(QObject *parent)
    : QObject(parent)
{
    // Default: <AppData>/BUGBOX/leaderboard.json (created if missing)
    QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (dir.isEmpty())
        dir = QStringLiteral(".");
    QDir().mkpath(dir);
    m_filePath = dir + QStringLiteral("/leaderboard.json");
}

void Leaderboard::setFilePath(const QString &path)
{
    m_filePath = path;
}

QString Leaderboard::filePath() const
{
    return m_filePath;
}

QVector<LeaderboardEntry> Leaderboard::loadAll() const
{
    QVector<LeaderboardEntry> entries;

    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly))
        return entries;                 // no file yet = empty leaderboard

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isArray())
        return entries;

    const QJsonArray array = doc.array();
    for (const QJsonValue &value : array) {
        const QJsonObject o = value.toObject();
        LeaderboardEntry e;
        e.name = o.value(QStringLiteral("name")).toString();
        e.score = o.value(QStringLiteral("score")).toInt();
        e.timeMs = static_cast<qint64>(o.value(QStringLiteral("timeMs")).toDouble());
        entries.push_back(e);
    }
    return entries;
}

bool Leaderboard::saveAll(const QVector<LeaderboardEntry> &entries) const
{
    QJsonArray array;
    for (const LeaderboardEntry &e : entries) {
        QJsonObject o;
        o.insert(QStringLiteral("name"), e.name);
        o.insert(QStringLiteral("score"), e.score);
        o.insert(QStringLiteral("timeMs"), static_cast<double>(e.timeMs));
        array.append(o);
    }

    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    file.write(QJsonDocument(array).toJson(QJsonDocument::Indented));
    return true;
}

void Leaderboard::submitScore(const QString &name, int score, qint64 timeMs)
{
    QVector<LeaderboardEntry> entries = loadAll();

    LeaderboardEntry e;
    e.name = name.trimmed().left(20);
    e.score = score;
    e.timeMs = timeMs;
    entries.push_back(e);

    sortEntries(entries);
    if (entries.size() > 100)           // keep the file small
        entries.resize(100);

    if (saveAll(entries))
        emit scoreSubmitted();
    else
        emit errorOccurred(tr("Could not save score to %1").arg(m_filePath));
}

void Leaderboard::fetchTop10()
{
    QVector<LeaderboardEntry> entries = loadAll();
    sortEntries(entries);
    if (entries.size() > 10)
        entries.resize(10);
    emit topFetched(entries);
}
