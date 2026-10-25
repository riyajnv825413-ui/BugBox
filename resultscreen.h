/**
 * resultscreen.h
 * -----------------------------------------------------------
 * ResultScreen — Phase 4: Final results display
 *
 * Shows:
 *   - Correct / incorrect banner
 *   - Final score
 *   - Total time taken
 *   - Number of hints used
 *   - Number of wrong submissions
 *   - Play Again and Home buttons
 *
 * Inherits QWidget (OOP: inheritance).
 * -----------------------------------------------------------
 */

#ifndef RESULTSCREEN_H
#define RESULTSCREEN_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

// -----------------------------------------------------------------------
// ResultScreen
// -----------------------------------------------------------------------
class ResultScreen : public QWidget
{
    Q_OBJECT

public:
    explicit ResultScreen(QWidget *parent = nullptr);
    ~ResultScreen() override = default;

    // Populate the screen with round results.
    // Call this before showing the screen.
    void setResults(int score, int timeTakenSeconds, int hintsUsed, int wrongCount,
                    int solvedCount, int totalChallenges);

signals:
    void playAgainRequested();
    void homeRequested();

private slots:
    void onPlayAgainClicked();
    void onHomeClicked();

private:
    void buildLayout();
    void applyStyles();

    // Widgets
    QLabel      *m_bannerLabel  { nullptr };
    QLabel      *m_scoreLabel   { nullptr };
    QLabel      *m_solvedLabel  { nullptr };
    QLabel      *m_timeLabel    { nullptr };
    QLabel      *m_hintsLabel   { nullptr };
    QLabel      *m_wrongsLabel  { nullptr };
    QLabel      *m_rankLabel    { nullptr };
    QPushButton *m_playAgainBtn { nullptr };
    QPushButton *m_homeBtn      { nullptr };
};

#endif // RESULTSCREEN_H
