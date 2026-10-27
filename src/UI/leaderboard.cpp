#include "leaderboard.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>
#include <QDir>
#include <algorithm>

Leaderboard::Leaderboard(QObject *parent)
    : QObject(parent)
{
    // Default path: AppData or local Documents directory to avoid write permission issues
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataDir); // Ensure directory exists
    m_filePath = dataDir + "/leaderboard.json";
}

void Leaderboard::setFilePath(const QString &path)
{
    m_filePath = path;
}

QString Leaderboard::filePath() const
{
    return m_filePath;
}

void Leaderboard::submitScore(const QString &name, int score, qint64 timeMs)
{
    QVector<LeaderboardEntry> entries = loadAll();

    // Create and add the new entry
    LeaderboardEntry newEntry;
    newEntry.name = name;
    newEntry.score = score;
    newEntry.timeMs = timeMs;
    entries.append(newEntry);

    // Save back to the JSON file
    if (saveAll(entries)) {
        emit scoreSubmitted();
    } else {
        emit errorOccurred("Failed to write score submission to data file.");
    }
}

void Leaderboard::fetchTop10()
{
    QVector<LeaderboardEntry> entries = loadAll();

    // Sort: Highest score first. If scores are equal, lower time (faster) wins.
    std::sort(entries.begin(), entries.end(), [](const LeaderboardEntry &a, const LeaderboardEntry &b) {
        if (a.score != b.score) {
            return a.score > b.score;
        }
        return a.timeMs < b.timeMs;
    });

    // Slice to keep only the top 10 items
    if (entries.size() > 10) {
        entries.resize(10);
    }

    emit topFetched(entries);
}

QVector<LeaderboardEntry> Leaderboard::loadAll() const
{
    QVector<LeaderboardEntry> entries;
    QFile file(m_filePath);

    if (!file.exists()) {
        return entries; // Return empty vector safely if file doesn't exist yet
    }

    if (!file.open(QIODevice::ReadOnly)) {
        return entries;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) {
        return entries;
    }

    QJsonArray array = doc.array();
    for (const QJsonValue &value : array) {
        if (value.isObject()) {
            QJsonObject obj = value.toObject();
            LeaderboardEntry entry;
            entry.name = obj.value("name").toString();
            entry.score = obj.value("score").toInt();
            entry.timeMs = obj.value("timeMs").toVariant().toLongLong();
            entries.append(entry);
        }
    }

    return entries;
}

bool Leaderboard::saveAll(const QVector<LeaderboardEntry> &entries) const
{
    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonArray array;
    for (const LeaderboardEntry &entry : entries) {
        QJsonObject obj;
        obj["name"] = entry.name;
        obj["score"] = entry.score;
        obj["timeMs"] = entry.timeMs;
        array.append(obj);
    }

    QJsonDocument doc(array);
    file.write(doc.toJson());
    file.close();
    return true;
}
