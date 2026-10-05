#include <iostream>

#include "BugGenerator.h"

using namespace std;

void displayBug(const BugData& bug)
{
    cout << "\n========================================\n";
    cout << "          BUG GENERATOR 3000\n";
    cout << "========================================\n";

    cout << "\nBug Type: "
         << bug.bugType << endl;

    cout << "Difficulty: ";

    switch (bug.difficulty)
    {
        case Difficulty::EASY:
            cout << "Easy";
            break;

        case Difficulty::MEDIUM:
            cout << "Medium";
            break;

        case Difficulty::HARD:
            cout << "Hard";
            break;
    }

    cout << "\n\nBuggy Code:\n";
    cout << "----------------------------------------\n";
    cout << bug.buggyCode << endl;
    cout << "----------------------------------------\n";

    cout << "\nExpected Output:\n";
    cout << "----------------------------------------\n";
    cout << bug.expectedOutput << endl;
    cout << "----------------------------------------\n";

    cout << "\nHint:\n";
    cout << bug.hint << endl;

    cout << "\n========================================\n";
}

int main()
{
    BugGenerator generator;

    int choice;

    cout << "\n========================================\n";
    cout << "          BUG GENERATOR 3000\n";
    cout << "========================================\n";

    cout << "\nChoose Difficulty:\n";
    cout << "1. Easy\n";
    cout << "2. Medium\n";
    cout << "3. Hard\n";

    cout << "\nEnter choice: ";
    cin >> choice;

    Difficulty difficulty;

    switch (choice)
    {
        case 1:
            difficulty = Difficulty::EASY;
            break;

        case 2:
            difficulty = Difficulty::MEDIUM;
            break;

        case 3:
            difficulty = Difficulty::HARD;
            break;

        default:
            cout << "\nInvalid choice!\n";
            return 1;
    }

    // Generate bug according to selected difficulty
    BugData bug = generator.generateBug(difficulty);

    // Display generated bug
    displayBug(bug);

    return 0;
}