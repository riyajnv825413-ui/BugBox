#include <iostream>

#include "../include/bug_engine/BugGenerator.h"

using namespace std;

int main()
{
    BugGenerator generator;

    BugData bug =
        generator.generateBug(Difficulty::MEDIUM);

    cout << "Bug Type: "
         << bug.bugType << endl;

    cout << "Buggy Code:\n"
         << bug.buggyCode << endl;

    cout << "Expected Output:\n"
         << bug.expectedOutput << endl;

    cout << "Hint:\n"
         << bug.hint << endl;

    // Do NOT display the solution in the actual game/application.

    return 0;
}