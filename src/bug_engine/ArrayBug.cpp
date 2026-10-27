#include "ArrayBug.h"

BugData ArrayBug::generate()
{
    BugData bug;

    bug.bugType = "Array Error";
    bug.difficulty = Difficulty::MEDIUM;

    bug.buggyCode =
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main()\n"
        "{\n"
        "    int numbers[5] = {10, 20, 30, 40, 50};\n\n"
        "    for (int i = 0; i <= 5; i++)\n"
        "    {\n"
        "        cout << numbers[i] << \" \";\n"
        "    }\n\n"
        "    return 0;\n"
        "}";

    bug.expectedOutput =
        "10 20 30 40 50";

    bug.hint =
        "Check the valid index range of the array.";

    bug.solution =
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main()\n"
        "{\n"
        "    int numbers[5] = {10, 20, 30, 40, 50};\n\n"
        "    for (int i = 0; i < 5; i++)\n"
        "    {\n"
        "        cout << numbers[i] << \" \";\n"
        "    }\n\n"
        "    return 0;\n"
        "}";

    return bug;
}