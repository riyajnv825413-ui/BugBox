#include "LogicBug.h"

BugData LogicBug::generate()
{
    BugData bug;

    bug.bugType = "Logic Error";
    bug.difficulty = Difficulty::EASY;

    bug.buggyCode =
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main()\n"
        "{\n"
        "    int a = 10;\n"
        "    int b = 20;\n\n"
        "    if (a > b)\n"
        "    {\n"
        "        cout << \"A is greater\";\n"
        "    }\n"
        "    else\n"
        "    {\n"
        "        cout << \"B is greater\";\n"
        "    }\n\n"
        "    return 0;\n"
        "}";

    bug.expectedOutput = "B is greater";

    bug.hint =
        "Check whether the comparison between a and b is correct.";

    bug.solution =
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main()\n"
        "{\n"
        "    int a = 10;\n"
        "    int b = 20;\n\n"
        "    if (a > b)\n"
        "    {\n"
        "        cout << \"A is greater\";\n"
        "    }\n"
        "    else\n"
        "    {\n"
        "        cout << \"B is greater\";\n"
        "    }\n\n"
        "    return 0;\n"
        "}";

    return bug;
}