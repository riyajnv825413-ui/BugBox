#include "SyntaxBug.h"

BugData SyntaxBug::generate()
{
    BugData bug;

    bug.bugType = "Syntax Error";
    bug.difficulty = Difficulty::EASY;

    bug.buggyCode =
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main()\n"
        "{\n"
        "    int number = 10\n"
        "    cout << number << endl;\n\n"
        "    return 0;\n"
        "}";

    bug.expectedOutput = "10";

    bug.hint =
        "Check the end of the variable declaration.";

    bug.solution =
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main()\n"
        "{\n"
        "    int number = 10;\n"
        "    cout << number << endl;\n\n"
        "    return 0;\n"
        "}";

    return bug;
}