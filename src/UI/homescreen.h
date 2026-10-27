/**
 * homescreen.h
 * -----------------------------------------------------------
 * HomeScreen — Phase 2: Home / Welcome screen
 *
 * Shows the BugBox title, subtitle, instructions, difficulty
 * selector, and Start button.
 *
 * Inherits QWidget (OOP: inheritance).
 * Emits startRequested() signal when the user clicks Start.
 * -----------------------------------------------------------
 */

#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

#include "bugchallenge.h"   // for Difficulty enum

// -----------------------------------------------------------------------
// HomeScreen
// -----------------------------------------------------------------------
class HomeScreen : public QWidget
{
    Q_OBJECT

public:
    explicit HomeScreen(QWidget *parent = nullptr);
    ~HomeScreen() override = default;

    // Returns the difficulty the user has currently selected.
    Difficulty selectedDifficulty() const;
    GameMode   selectedGameMode()   const;

signals:
    // Emitted when the user clicks "Start Debugging".
    void startRequested(const QString &playerName,Difficulty difficulty, GameMode mode);

private slots:
    void onStartClicked();
     void onLeaderboardButtonClicked();

private:
    // --- Helpers --------------------------------------------------------
    void buildLayout();
    void applyStyles();

    // --- Widgets --------------------------------------------------------
    QLabel      *m_titleLabel      { nullptr };
    QLabel      *m_subtitleLabel   { nullptr };
    QLabel      *m_instructLabel   { nullptr };
    QComboBox   *m_difficultyCombo { nullptr };
    QComboBox   *m_gameModeCombo   { nullptr };
    QPushButton *m_startButton     { nullptr };
    QLineEdit *m_nameLineEdit { nullptr };
    QLabel *m_nameLabel { nullptr };
};

#endif // HOMESCREEN_H
