#include "ConditionBug.h"

BugData ConditionBug::generate()
{
    BugData bug;

    bug.bugType = "Condition Error";
    bug.difficulty = Difficulty::MEDIUM;

    bug.buggyCode =
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main()\n"
        "{\n"
        "    int age = 20;\n\n"
        "    if (age < 18)\n"
        "    {\n"
        "        cout << \"Adult\";\n"
        "    }\n"
        "    else\n"
        "    {\n"
        "        cout << \"Minor\";\n"
        "    }\n\n"
        "    return 0;\n"
        "}";

    bug.expectedOutput = "Adult";

    bug.hint =
        "Check the condition used to determine whether the person is an adult.";

    bug.solution =
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main()\n"
        "{\n"
        "    int age = 20;\n\n"
        "    if (age >= 18)\n"
        "    {\n"
        "        cout << \"Adult\";\n"
        "    }\n"
        "    else\n"
        "    {\n"
        "        cout << \"Minor\";\n"
        "    }\n\n"
        "    return 0;\n"
        "}";

    return bug;
}