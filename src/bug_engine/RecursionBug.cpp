#include "RecursionBug.h"

BugData RecursionBug::generate()
{
    BugData bug;

    bug.bugType = "Recursion Error";
    bug.difficulty = Difficulty::HARD;

    bug.buggyCode =
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int factorial(int n)\n"
        "{\n"
        "    if (n == 0)\n"
        "        return 1;\n\n"
        "    return n * factorial(n);\n"
        "}\n\n"
        "int main()\n"
        "{\n"
        "    cout << factorial(5);\n"
        "    return 0;\n"
        "}";

    bug.expectedOutput = "120";

    bug.hint =
        "Check how the value of n changes during the recursive call.";

    bug.solution =
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int factorial(int n)\n"
        "{\n"
        "    if (n == 0)\n"
        "        return 1;\n\n"
        "    return n * factorial(n - 1);\n"
        "}\n\n"
        "int main()\n"
        "{\n"
        "    cout << factorial(5);\n"
        "    return 0;\n"
        "}";

    return bug;
}