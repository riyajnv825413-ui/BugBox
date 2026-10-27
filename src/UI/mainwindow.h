#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
/**
 * mainwindow.h
 * -----------------------------------------------------------
 * MainWindow — Phase 5: Navigation controller
 *
 * Uses a QStackedWidget to switch between:
 *   Index 0 — HomeScreen
 *   Index 1 — DebugScreen
 *   Index 2 — ResultScreen
 *
 * Inherits QMainWindow (OOP: inheritance).
 * Owns the three screen widgets (OOP: composition).
 * Owns a BugChallengeAdapter (OOP: composition).
 * -----------------------------------------------------------
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

#include "homescreen.h"
#include "debugscreen.h"
#include "resultscreen.h"
#include "bugchallengeadapter.h"
#include "CompilerEngine.h"

// -----------------------------------------------------------------------
// MainWindow
// -----------------------------------------------------------------------
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

    // Screen indices — named constants (avoid magic numbers)
    static constexpr int kHomeIndex   = 0;
    static constexpr int kDebugIndex  = 1;
    static constexpr int kResultIndex = 2;

private slots:
    void onStartRequested(Difficulty difficulty, GameMode mode);
    void onSessionFinished(int score, int timeSecs, int hints, int wrongs,
                           int solved, int totalChallenges);
    void onHomeFromDebug();
    void onHomeFromResult();
    void onPlayAgain();

private:
    void setupScreens();
    void showScreen(int index);

    // --- Widgets (OOP: composition) ------------------------------------
    QStackedWidget       *m_stack   { nullptr };
    HomeScreen           *m_home    { nullptr };
    DebugScreen          *m_debug   { nullptr };
    ResultScreen         *m_result  { nullptr };

    // --- Mock adapter (OOP: composition) --------------------------------
    // *** Replace BugChallengeAdapter with the real module when available ***
    BugChallengeAdapter  *m_adapter { nullptr };

    // --- Compilation engine (teammate module) ---------------------------
    CompilerEngine       *m_compilerEngine { nullptr };

    // Remember the last selected difficulty for "Play Again".
    Difficulty            m_lastDifficulty { Difficulty::Easy };
    GameMode              m_lastGameMode   { GameMode::FullRun };
};

#endif // MAINWINDOW_H

