#include <iostream>

#include "../include/bug_engine/BugGenerator.h"

using namespace std;

int main()
{
    BugGenerator generator;

    cout << "Testing Easy...\n";

    for (int i = 0; i < 10; i++)
    {
        BugData bug =
            generator.generateBug(Difficulty::EASY);

        cout << bug.bugType << endl;
    }

    cout << "\nTesting Medium...\n";

    for (int i = 0; i < 10; i++)
    {
        BugData bug =
            generator.generateBug(Difficulty::MEDIUM);

        cout << bug.bugType << endl;
    }

    cout << "\nTesting Hard...\n";

    for (int i = 0; i < 10; i++)
    {
        BugData bug =
            generator.generateBug(Difficulty::HARD);

        cout << bug.bugType << endl;
    }

    return 0;
}