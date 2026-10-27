#include "NestedLoopBug.h"

BugData NestedLoopBug::generate()
{
    BugData bug;

    bug.bugType = "Nested Loop Error";
    bug.difficulty = Difficulty::HARD;

    bug.buggyCode =
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main()\n"
        "{\n"
        "    for (int i = 1; i <= 3; i++)\n"
        "    {\n"
        "        for (int j = 1; j <= 3; j++)\n"
        "        {\n"
        "            cout << i + j << \" \";\n"
        "        }\n"
        "        cout << endl;\n"
        "    }\n\n"
        "    return 0;\n"
        "}";

    bug.expectedOutput =
        "1 2 3\n"
        "2 4 6\n"
        "3 6 9";

    bug.hint =
        "Look carefully at the operation performed using the two loop variables.";

    bug.solution =
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main()\n"
        "{\n"
        "    for (int i = 1; i <= 3; i++)\n"
        "    {\n"
        "        for (int j = 1; j <= 3; j++)\n"
        "        {\n"
        "            cout << i * j << \" \";\n"
        "        }\n"
        "        cout << endl;\n"
        "    }\n\n"
        "    return 0;\n"
        "}";

    return bug;
}