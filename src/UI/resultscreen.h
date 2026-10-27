#ifndef RESULTSCREEN_H
#define RESULTSCREEN_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class ResultScreen : public QWidget
{
    Q_OBJECT

public:
    explicit ResultScreen(QWidget *parent = nullptr);
    void setResults(const QString &playerName,int score, int timeTakenSeconds, int hintsUsed, int wrongCount, int solvedCount, int totalChallenges);


signals:
    void playAgainRequested();
    void homeRequested();

private slots:
    void onPlayAgainClicked();
    void onHomeClicked();
    void onLeaderboardClicked(); // <-- Verify this says 'onLeaderboardClicked' without 'Button' in it

private:
    void buildLayout();
    void applyStyles();

    QLabel *m_bannerLabel;
    QLabel *m_scoreLabel;
    QLabel *m_solvedLabel;
    QLabel *m_timeLabel;
    QLabel *m_hintsLabel;
    QLabel *m_wrongsLabel;
    QLabel *m_rankLabel;

    QPushButton *m_homeBtn;
    QPushButton *m_playAgainBtn;
    QPushButton *m_leaderboardBtn; // <-- Verify this has the 'm_' prefix and matches your cpp error spot
    QString m_storedPlayerName;
    int m_storedScore = 0;
    int m_storedTimeSeconds = 0;
};

#endif // RESULTSCREEN_H
