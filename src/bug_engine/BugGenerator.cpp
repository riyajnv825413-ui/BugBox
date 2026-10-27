#include "BugGenerator.h"
#include "RecursionBug.h"
#include "LogicBug.h"
#include "SyntaxBug.h"
#include "LoopBug.h"

#include "ArrayBug.h"
#include "ConditionBug.h"

#include "NestedLoopBug.h"

#include <random>

BugData BugGenerator::generateBug(Difficulty difficulty)
{
    // Create random number generator
    static std::random_device rd;
    static std::mt19937 generator(rd());

    // =========================================
    // EASY
    // =========================================

    if (difficulty == Difficulty::EASY)
    {
        std::uniform_int_distribution<int> distribution(0, 2);

        int choice = distribution(generator);

        switch (choice)
        {
            case 0:
            {
                LogicBug bug;
                return bug.generate();
            }

            case 1:
            {
                SyntaxBug bug;
                return bug.generate();
            }

            case 2:
            {
                LoopBug bug;
                return bug.generate();
            }
        }
    }

    // =========================================
    // MEDIUM
    // =========================================

    if (difficulty == Difficulty::MEDIUM)
    {
        std::uniform_int_distribution<int> distribution(0, 1);

        int choice = distribution(generator);

        switch (choice)
        {
            case 0:
            {
                ArrayBug bug;
                return bug.generate();
            }

            case 1:
            {
                ConditionBug bug;
                return bug.generate();
            }
        }
    }

    // =========================================
    // HARD
    // =========================================

   // =========================================
// HARD
// =========================================

if (difficulty == Difficulty::HARD)
{
    std::uniform_int_distribution<int> distribution(0, 1);

    int choice = distribution(generator);

    switch (choice)
    {
        case 0:
        {
            NestedLoopBug bug;
            return bug.generate();
        }

        case 1:
        {
            RecursionBug bug;
            return bug.generate();
        }
    }
}
    // =========================================
    // FALLBACK
    // =========================================

    LogicBug bug;
    return bug.generate();
}