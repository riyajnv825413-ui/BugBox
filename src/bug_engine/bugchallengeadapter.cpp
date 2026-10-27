/**
 * bugchallengeadapter.cpp
 * -----------------------------------------------------------
 * *** MOCK IMPLEMENTATION — Replace with real module calls ***
 *
 * Each BugChallenge is constructed using the full constructor
 * (OOP: constructor-based initialisation).
 * -----------------------------------------------------------
 */

#include "bugchallengeadapter.h"

// -----------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------
BugChallengeAdapter::BugChallengeAdapter()
{
    loadMockChallenges();
}

// -----------------------------------------------------------------------
// challengesForDifficulty
// Filters the internal vector by difficulty and returns a copy.
// -----------------------------------------------------------------------
QVector<BugChallenge> BugChallengeAdapter::challengesForDifficulty(Difficulty d) const
{
    QVector<BugChallenge> result;
    for (const BugChallenge &ch : m_challenges) {
        if (ch.difficulty() == d) {
            result.append(ch);
        }
    }
    return result;
}

// -----------------------------------------------------------------------
// loadMockChallenges — MOCK DATA
// -----------------------------------------------------------------------
void BugChallengeAdapter::loadMockChallenges()
{
    // ===== EASY CHALLENGES ==============================================

    m_challenges.append(BugChallenge(
        "Off-by-one loop",
        "The loop below should print numbers 1 through 5.\n"
        "Find and fix the bug so it prints exactly:\n  1 2 3 4 5",
        // --- buggy code ---
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main() {\n"
        "    for (int i = 1; i < 5; i++) {  // bug: stops at 4\n"
        "        cout << i << \" \";\n"
        "    }\n"
        "    cout << endl;\n"
        "    return 0;\n"
        "}",
        // --- expected output ---
        "1 2 3 4 5",
        // --- solution (fixed code) ---
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main() {\n"
        "    for (int i = 1; i <= 5; i++) {\n"
        "        cout << i << \" \";\n"
        "    }\n"
        "    cout << endl;\n"
        "    return 0;\n"
        "}",
        // --- hint ---
        "Check the loop condition: should it be < 5 or <= 5?",
        Difficulty::Easy
    ));

    m_challenges.append(BugChallenge(
        "Missing semicolon",
        "This program should print \"Hello, World!\".\n"
        "There is a syntax error — find it!",
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main() {\n"
        "    cout << \"Hello, World!\" << endl\n"
        "    return 0;\n"
        "}",
        "Hello, World!",
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main() {\n"
        "    cout << \"Hello, World!\" << endl;\n"
        "    return 0;\n"
        "}",
        "Look at the end of the cout line. Does every statement end with a semicolon?",
        Difficulty::Easy
    ));

    m_challenges.append(BugChallenge(
        "Wrong comparison operator",
        "The function should return true when a equals b.\n"
        "But there is a classic assignment-vs-comparison bug.",
        "bool areEqual(int a, int b) {\n"
        "    if (a = b)   // bug here\n"
        "        return true;\n"
        "    return false;\n"
        "}",
        "(no output — logic error)",
        "bool areEqual(int a, int b) {\n"
        "    if (a == b)\n"
        "        return true;\n"
        "    return false;\n"
        "}",
        "In C++, a single = assigns a value. To compare, you need ==.",
        Difficulty::Easy
    ));

    // ===== MEDIUM CHALLENGES ============================================

    m_challenges.append(BugChallenge(
        "Array out-of-bounds",
        "The code below sums the elements of an array of size 5.\n"
        "It should print 15 but crashes instead. Why?",
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main() {\n"
        "    int arr[5] = {1, 2, 3, 4, 5};\n"
        "    int sum = 0;\n"
        "    for (int i = 0; i <= 5; i++) {  // bug here\n"
        "        sum += arr[i];\n"
        "    }\n"
        "    cout << sum << endl;\n"
        "    return 0;\n"
        "}",
        "15",
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main() {\n"
        "    int arr[5] = {1, 2, 3, 4, 5};\n"
        "    int sum = 0;\n"
        "    for (int i = 0; i < 5; i++) {\n"
        "        sum += arr[i];\n"
        "    }\n"
        "    cout << sum << endl;\n"
        "    return 0;\n"
        "}",
        "An array of size 5 has valid indices 0 through 4. What does i <= 5 do on the last iteration?",
        Difficulty::Medium
    ));

    m_challenges.append(BugChallenge(
        "Infinite while loop",
        "This loop should count down from 5 to 1 and then stop.\n"
        "Instead it runs forever. Fix the bug.",
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main() {\n"
        "    int n = 5;\n"
        "    while (n > 0) {\n"
        "        cout << n << \" \";\n"
        "        // bug: n never changes\n"
        "    }\n"
        "    cout << endl;\n"
        "    return 0;\n"
        "}",
        "5 4 3 2 1",
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main() {\n"
        "    int n = 5;\n"
        "    while (n > 0) {\n"
        "        cout << n << \" \";\n"
        "        n--;\n"
        "    }\n"
        "    cout << endl;\n"
        "    return 0;\n"
        "}",
        "What has to change inside the loop body so that the condition eventually becomes false?",
        Difficulty::Medium
    ));

    m_challenges.append(BugChallenge(
        "Uninitialized variable",
        "The program should compute the factorial of 5 (= 120).\n"
        "It gives a wrong answer. Find the uninitialized variable.",
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main() {\n"
        "    int result;   // bug: not initialised\n"
        "    for (int i = 1; i <= 5; i++) {\n"
        "        result *= i;\n"
        "    }\n"
        "    cout << result << endl;\n"
        "    return 0;\n"
        "}",
        "120",
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main() {\n"
        "    int result = 1;\n"
        "    for (int i = 1; i <= 5; i++) {\n"
        "        result *= i;\n"
        "    }\n"
        "    cout << result << endl;\n"
        "    return 0;\n"
        "}",
        "For a multiplication accumulator, what should the starting value be?",
        Difficulty::Medium
    ));

    // ===== HARD CHALLENGES ===============================================

    m_challenges.append(BugChallenge(
        "Dangling pointer",
        "The function returns a pointer to a local variable.\n"
        "This is undefined behaviour. Fix it using dynamic allocation\n"
        "or a static / global variable.",
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int* getNumber() {\n"
        "    int x = 42;  // local variable\n"
        "    return &x;   // bug: dangling pointer!\n"
        "}\n\n"
        "int main() {\n"
        "    int* p = getNumber();\n"
        "    cout << *p << endl;\n"
        "    return 0;\n"
        "}",
        "42",
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int* getNumber() {\n"
        "    int* x = new int(42);\n"
        "    return x;   // caller must delete\n"
        "}\n\n"
        "int main() {\n"
        "    int* p = getNumber();\n"
        "    cout << *p << endl;\n"
        "    delete p;\n"
        "    return 0;\n"
        "}",
        "Local variables are destroyed when the function returns. Who owns the memory after the function exits?",
        Difficulty::Hard
    ));

    m_challenges.append(BugChallenge(
        "Stack overflow via recursion",
        "The Fibonacci function below recurses without a proper\n"
        "base case and overflows the stack. Fix the base cases.",
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int fib(int n) {\n"
        "    return fib(n-1) + fib(n-2);  // bug: no base case\n"
        "}\n\n"
        "int main() {\n"
        "    cout << fib(6) << endl;\n"
        "    return 0;\n"
        "}",
        "8",
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int fib(int n) {\n"
        "    if (n <= 0) return 0;\n"
        "    if (n == 1) return 1;\n"
        "    return fib(n-1) + fib(n-2);\n"
        "}\n\n"
        "int main() {\n"
        "    cout << fib(6) << endl;\n"
        "    return 0;\n"
        "}",
        "Every recursive function needs one or more base cases that stop the recursion. What are the two base cases for Fibonacci?",
        Difficulty::Hard
    ));

m_challenges.append(BugChallenge(
        "Integer overflow",
        "The program tries to compute 2^31 using int arithmetic.\n"
        "It produces a wrong (negative) answer. Fix the type.",
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main() {\n"
        "    int result = 1;\n"
        "    for (int i = 0; i < 31; i++) {\n"
        "        result *= 2;\n"
        "    }\n"
        "    cout << result << endl;\n"
        "    return 0;\n"
        "}",
        "2147483648",
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main() {\n"
        "    long long result = 1;\n"
        "    for (int i = 0; i < 31; i++) {\n"
        "        result *= 2;\n"
        "    }\n"
        "    cout << result << endl;\n"
        "    return 0;\n"
        "}",
        "A 32-bit signed int can only hold values up to ~2.1 billion. 2^31 = 2,147,483,648 — what type can hold larger numbers?",
        Difficulty::Hard
    ));
}
