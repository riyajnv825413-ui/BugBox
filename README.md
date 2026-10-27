# 🐛 BUGBOX

> **Find the bug. Fix the code. Beat the clock.**

A college project desktop game built with **C++**, **Qt 6 Widgets**, and **CMake**.
BUGBOX shows buggy C++ programs; the player fixes the bug, and the app compiles
and runs the code to check the real output.

---

## 📋 Project Overview

BUGBOX combines four team contributions into one application:

| Module | Owner | What it does |
|---|---|---|
| GUI, scoring, timer | **Rishika** | Home / Debug / Result screens, score rules, countdown |
| Bug generator engine | **Riya** | Random bug templates (`BugGenerator`, `LoopBug`, `ArrayBug`, …) |
| Compiler evaluation engine | **Teammate compiler module** | Compiles submissions with MinGW g++ and compares real stdout |
| Leaderboard | **Teammate leaderboard module** | Top-10 scores stored in a local JSON file |

The GUI's `BugChallengeAdapter` now combines the hand-written curated
challenges with two randomised challenges from the bug engine for each
difficulty. A small bridge (`bugenginebridge.*`) converts the engine's
`std::string`/global `Difficulty` API into the Qt model without a name clash.

---

## 🗂️ File Structure

```
bugboxwidget/
├── main.cpp                       # Entry point (sets app/org name)
├── CMakeLists.txt                 # Build configuration
│
├── mainwindow.h / .cpp            # Navigation + leaderboard + score saving
├── homescreen.h / .cpp            # Home screen (Start, Leaderboard)
├── debugscreen.h / .cpp           # Game screen (Run, Hint, Submit, Next)
├── resultscreen.h / .cpp          # Result screen (score, rank, leaderboard)
│
├── bugchallenge.h / .cpp          # Challenge data model
├── bugchallengeadapter.h / .cpp   # Curated + generated challenge adapter
├── bugengineadapter.h / .cpp      # BugData -> BugChallenge adapter
├── bugenginebridge.h / .cpp       # Qt <-> standalone bug engine bridge
├── scoremanager.h / .cpp          # Score logic (100 start, -10 wrong, -5 hint)
│
├── CompilerEngine.h / .cpp        # Real g++ compilation + execution
├── leaderboard.h / .cpp           # JSON top-10 storage
├── leaderboarddialog.h / .cpp     # Leaderboard table dialog
│
├── include/bug_engine/            # Teammate bug engine headers
├── src/bug_engine/                # Teammate bug engine implementations
├── Tests/                         # Teammate engine tests
├── docs/                          # Teammate engine documentation
└── data/Templates/                # Placeholder template folders
```

---

## 🏗️ How to Build

### Prerequisites
- Qt 6.5 or later with the **MinGW 64-bit** kit
- CMake 3.19 or later
- MinGW g++ 13.x (installed with the Qt kit under `C:\Qt\Tools\mingw...`)

### Build in Qt Creator (easiest)
1. Open Qt Creator.
2. **File → Open File or Project** → select `CMakeLists.txt`.
3. Choose the **Desktop Qt 6.x MinGW 64-bit** kit.
4. Click **Build** (or `Ctrl+B`), then **Run** (`Ctrl+R`).

If Qt Creator asks to configure the project, accept — the old
`build/Desktop_Qt_...` folder was removed during earlier troubleshooting.
A working command-line build already exists in `build/`.

### Build from the command line (PowerShell)
```powershell
$env:PATH = "C:\Qt\Tools\CMake_64\bin;C:\Qt\Tools\mingw1310_64\bin;C:\Qt\6.12.0\mingw_64\bin;$env:PATH"

cd "C:\Users\Welcome\Documents\bugboxwidget"
cmake -G "MinGW Makefiles" -S . -B build `
      -DCMAKE_PREFIX_PATH="C:/Qt/6.12.0/mingw_64" `
      -DCMAKE_MAKE_PROGRAM="C:/Qt/Tools/mingw1310_64/bin/mingw32-make.exe" `
      -DCMAKE_BUILD_TYPE=Release

cmake --build build -j4
.\build\bugboxwidget.exe
```

---

## 🎮 How to Play

1. Launch the app → **Home** screen.
2. Choose **Easy / Medium / Hard** and **Full Run / Quick Round**.
3. Click **▶ Start Debugging**.
4. Read the buggy code, fix it in the editor, then:
   - **▶ Run** — compiles and runs the code, shows the real output.
   - **💡 Hint** — reveals the hint (−5 points, once per challenge).
   - **✔ Submit** — validates the answer and records the score.
   - **Next Bug →** — advances after a correct answer.
5. At the end, the **Result** screen shows score, time, rank, and stats.
6. **🏆 Leaderboard** on Home/Result shows the top 10 saved scores.

### Validation rules
- The submitted code is written to a temporary folder and compiled with
  `g++ -std=c++17`.
- The compiled program runs with a **2-second time limit** and a
  **10-second compile limit**.
- Its stdout is compared with the challenge's expected output.
- Line endings, leading/trailing blank lines, and trailing spaces are ignored;
  comments do **not** affect compilation, so harmless comments are accepted.
- Compilation errors, crashes, timeouts, and wrong output are rejected.
- If g++ cannot be found, the game falls back to the old normalised-text
  comparison and clearly says so in the output panel.

> **Security note:** user code is executed as a normal desktop process with a
> time limit only. This is suitable for a college demo, not a public judge.

---

## 🔤 Compiler Discovery

`CompilerEngine` looks for g++ in this order:

1. `BUGBOX_COMPILER` environment variable
2. A path passed to the constructor
3. `PATH`
4. `BUGBOX.exe` folder / `compiler\g++.exe` / `mingw\bin\g++.exe`
5. `C:\Qt\Tools\mingw*\bin\g++.exe`
6. `C:\msys64\ucrt64\bin\g++.exe`, `C:\MinGW\bin\g++.exe`, and common Code::Blocks paths

Example override:
```powershell
$env:BUGBOX_COMPILER = "C:\Qt\Tools\mingw1310_64\bin\g++.exe"
```

---

## 🧮 Scoring Rules

| Event | Points |
|---|---|
| Starting score | 100 |
| Hint used (once per challenge) | −5 |
| Wrong submission | −10 |
| Score floor | 0 (never negative) |

### Rank Badges
| Score | Rank |
|---|---|
| 100 | ⭐ Perfect Score — Bug Slayer! |
| 80–99 | 🥇 Debug Master |
| 60–79 | 🥈 Code Inspector |
| 40–59 | 🥉 Bug Hunter |
| 0–39 | 🔰 Apprentice Debugger |

---

## 🏆 Leaderboard

- Implemented by the teammate leaderboard module.
- Scores are saved to:
  `%APPDATA%\BUGBOX\BUGBOX\leaderboard.json`
- Sorted by highest score, then fastest time.
- The name prompt appears after a finished round in which at least one
  challenge was solved.

---

## ✅ Verification Performed

A headless integration test compiled **every challenge solution** (curated and
generated across Easy/Medium/Hard) and confirmed:

- every fixed solution → `Accepted`;
- every original buggy version → rejected
  (`Wrong Answer`, `Compilation Error`, `Runtime Error`, or `Time Limit Exceeded`);
- leaderboard sorting and JSON persistence work.

A Qt GUI test simulated the original complaint:
keeping the buggy `// bug: stops at 4` comment and changing only `< 5` to
`<= 5`. The built app compiled it, printed `1 2 3 4 5`, accepted it, and kept
the score at **100**.

---

## 📦 Portable Windows Build

A deployed portable folder and ZIP are available at:

- Folder: `C:\Users\Welcome\Documents\BUGBOX-Windows-x64`
- ZIP: `C:\Users\Welcome\Documents\BUGBOX-Windows-x64.zip` (~18 MB)

The folder contains `BUGBOX.exe`, all required Qt DLLs/plugins, and a
`README.txt`. It runs without Qt installed. **Real compilation still needs
MinGW g++** on the machine (see above).

> A Qt Widgets app is a desktop program, so it cannot be hosted at a website
> URL. The GitHub repository is the source URL; the ZIP/GitHub Release is the
> distributable build.

---

## 🔗 Repository

- Repo: <https://github.com/riyajnv825413-ui/BugBox>
- Team branch: `rishika`
- Integration branch: `integration` (merged into `rishika` after testing)

`main` is the team's protected branch and was not modified.

---

## 👤 Team Responsibilities

| Module | Owner |
|---|---|
| GUI, Scoring, Timer | Rishika |
| Bug Generation | Riya |
| Code Evaluation | Compiler teammate |
| Leaderboard | Leaderboard teammate |

---

*BUGBOX — College Project | Qt 6 Widgets | C++ | CMake*
