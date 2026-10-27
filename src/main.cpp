#include <iostream>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "bug_engine/BugGenerator.h" // Points directly into engine folder
#include "UI/mainwindow.h"           // Points directly into UI folder


using namespace std;

// This will globally hold the generated bug info so the UI can read it on launch
BugData g_startingBug;
bool g_hasCustomBug = false;

void displayBugInConsole(const BugData& bug)
{
    cout << "\n========================================\n";
    cout << "          BUG GENERATOR 3000\n";
    cout << "========================================\n";
    cout << "\nBug Type: " << bug.bugType << endl;
    cout << "Difficulty: ";

    switch (bug.difficulty)
    {
        case Difficulty::EASY:   cout << "Easy"; break;
        case Difficulty::MEDIUM: cout << "Medium"; break;
        case Difficulty::HARD:   cout << "Hard"; break;
    }

    cout << "\n\nBuggy Code:\n----------------------------------------\n" << bug.buggyCode << "\n----------------------------------------\n";
    cout << "\nExpected Output:\n----------------------------------------\n" << bug.expectedOutput << "\n----------------------------------------\n";
    cout << "\nHint:\n" << bug.hint << endl;
    cout << "\n========================================\n";
    cout << "\nLaunching Graphical UI Panel now...\n";
}

int main(int argc, char *argv[])
{
    // =======================================================================
    // 1. PHASE 1: YOUR BUG GENERATION CONSOLE ENGINE
    // =======================================================================
    BugGenerator generator;
    int choice;

    cout << "\n========================================\n";
    cout << "          BUG GENERATOR 3000\n";
    cout << "========================================\n";
    cout << "\nChoose Difficulty:\n";
    cout << "1. Easy\n2. Medium\n3. Hard\n";
    cout << "\nEnter choice: ";
    
    if (cin >> choice) {
        Difficulty difficulty;
        bool valid = true;
        switch (choice)
        {
            case 1:  difficulty = Difficulty::EASY; break;
            case 2:  difficulty = Difficulty::MEDIUM; break;
            case 3:  difficulty = Difficulty::HARD; break;
            default:
                cout << "\nInvalid choice! Defaulting to Easy.\n";
                difficulty = Difficulty::EASY;
                break;
        }

        // Generate the structural data using your engine
        g_startingBug = generator.generateBug(difficulty);
        g_hasCustomBug = true;

        // Print it to terminal output cleanly
        displayBugInConsole(g_startingBug);
    }

    // =======================================================================
    // 2. PHASE 2: QT GRAPHICAL FRAMEWORK LAUNCH
    // =======================================================================
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "bugboxwidget_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w;
    w.show();
    
    return QApplication::exec();
}
