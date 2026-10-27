#include "LoopBug.h"

BugData LoopBug::generate()
{
    BugData bug;

    bug.bugType = "Loop Error";
    bug.difficulty = Difficulty::EASY;

    bug.buggyCode =
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main()\n"
        "{\n"
        "    for (int i = 1; i <= 5; i--)\n"
        "    {\n"
        "        cout << i << \" \";\n"
        "    }\n\n"
        "    return 0;\n"
        "}";

    bug.expectedOutput = "1 2 3 4 5";

    bug.hint =
        "Look at how the loop variable changes after every iteration.";

    bug.solution =
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main()\n"
        "{\n"
        "    for (int i = 1; i <= 5; i++)\n"
        "    {\n"
        "        cout << i << \" \";\n"
        "    }\n\n"
        "    return 0;\n"
        "}";

    return bug;
}