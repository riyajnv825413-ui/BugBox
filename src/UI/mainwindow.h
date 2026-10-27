#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QVector>
#include "BugData.h"  // THIS LINE FIXES THE RED LINE UNDER DIFFICULTY
#include "homescreen.h"
#include "debugscreen.h"
#include "resultscreen.h"
#include "bugchallengeadapter.h"
#include "CompilerEngine.h"

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

    // Screen indices — named constants (avoid magic numbers)
    static constexpr int kHomeIndex   = 0;
    static constexpr int kDebugIndex  = 1;
    static constexpr int kResultIndex = 2;

private slots:
    void onStartRequested(const QString &playerName,Difficulty difficulty, GameMode mode);
    void onSessionFinished(int score, int timeSecs, int hints, int wrongs,
                           int solved, int totalChallenges);
    void onHomeFromDebug();
    void onHomeFromResult();
    void onPlayAgain();

private:
    void setupScreens();
    void showScreen(int index);

    // --- Core UI Form Layout pointer -----------------------------------
    Ui::MainWindow *ui;

    // --- Widgets (OOP: composition) ------------------------------------
    QStackedWidget       *m_stack   { nullptr };
    HomeScreen           *m_home    { nullptr };
    DebugScreen          *m_debug   { nullptr };
    ResultScreen         *m_result  { nullptr };

    // --- Mock adapter (OOP: composition) --------------------------------
    BugChallengeAdapter  *m_adapter { nullptr };

    // --- Compilation engine (teammate module) ---------------------------
    CompilerEngine       *m_compilerEngine { nullptr };

    // Remember the last selected difficulty for "Play Again".
    Difficulty            m_lastDifficulty {Difficulty::EASY };
    GameMode              m_lastGameMode   { GameMode::FullRun };
    QString m_playerName; 
};

#endif // MAINWINDOW_H
