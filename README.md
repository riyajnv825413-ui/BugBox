# 🐛 BUGBOX

> **Find the bug. Fix the code. Beat the clock.**

A college project GUI application built with **C++**, **Qt 6 Widgets**, and **CMake**.

---

## 📋 Project Overview

BUGBOX is an interactive debugging game where players are shown snippets of buggy C++ code and must find and fix the bug before the timer runs out. The game tracks score, time, hints used, and wrong submissions.

---

## 🗂️ File Structure

```
bugboxwidget/
├── main.cpp                     # Entry point
├── CMakeLists.txt               # Build configuration
│
├── mainwindow.h / .cpp          # Navigation controller (QStackedWidget)
│
├── homescreen.h / .cpp          # Phase 2: Home / Welcome screen
├── debugscreen.h / .cpp         # Phase 3: Debug / Game screen
├── resultscreen.h / .cpp        # Phase 4: Result screen
│
├── bugchallenge.h / .cpp        # Data class: one bug challenge (OOP)
├── bugchallengeadapter.h / .cpp # *** MOCK *** adapter with sample data
├── scoremanager.h / .cpp        # Score logic class (OOP, QObject)
│
└── mainwindow.ui                # Kept for Qt Creator compatibility (unused)
```

---

## 🏗️ How to Build

### Prerequisites
- Qt 6.5 or later (with MinGW 64-bit kit)
- CMake 3.19 or later
- Qt Creator (recommended) **or** command-line build

### Build in Qt Creator (Easiest)
1. Open Qt Creator
2. **File → Open File or Project** → select `CMakeLists.txt`
3. Choose the **Desktop Qt 6.x MinGW 64-bit Debug** kit
4. Click **Build** (hammer icon) or press **Ctrl+B**
5. Click **Run** (green play button) or press **Ctrl+R**

### Build from Command Line (PowerShell)
```powershell
# Navigate to the existing build directory
cd "C:\Users\Welcome\Documents\bugboxwidget\build\Desktop_Qt_6_12_0_MinGW_64_bit_Debug"

# Re-run CMake to pick up new source files
C:\Qt\Tools\CMake_64\bin\cmake.exe ..\..\

# Build
C:\Qt\Tools\mingw1310_64\bin\mingw32-make.exe -j4

# Run
.\bugboxwidget.exe
```

---

## 🎮 How to Play

1. Launch the app → **Home Screen** appears
2. Read the instructions
3. Select a difficulty: **Easy**, **Medium**, or **Hard**
4. Select a game mode: **Full Run** (all challenges) or **Quick Round** (one challenge)
5. Click **▶ Start Debugging**
6. On the **Debug Screen**:
   - Read the challenge description
   - Find the bug in the C++ code editor
   - Fix the code directly in the editor
   - Click **▶ Run** for a dry-run check (no compilation for security)
   - Click **✔ Submit** to submit your answer
   - Click **💡 Hint** if stuck (costs 5 points, one-time per challenge)
   - Each wrong submission costs 10 points
7. When done → **Result Screen** shows your score, time, rank, and stats
8. Click **▶ Play Again** or **🏠 Home**

---

## 📐 OOP Design

| Class | OOP Concept | Purpose |
|---|---|---|
| `BugChallenge` | Encapsulation, Constructors | Data model for one challenge |
| `BugChallengeAdapter` | Adapter pattern, Composition | Loads challenges (MOCK) |
| `ScoreManager` | Single Responsibility, Inheritance (QObject) | Score tracking with guards |
| `HomeScreen` | Inheritance (QWidget) | Welcome / difficulty screen |
| `DebugScreen` | Inheritance (QWidget), Composition | Main game screen |
| `ResultScreen` | Inheritance (QWidget) | Results display |
| `MainWindow` | Inheritance (QMainWindow), Composition | Navigation controller |

**Containers used:** `QVector<BugChallenge>` in `BugChallengeAdapter` and `DebugScreen`.

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

## ⏱️ Timer

- Starts when a challenge loads
- Updates every second via `QTimer`
- Stops when round ends (correct submit or session end)
- Multiple timer starts are prevented (`m_timer->stop()` before `start()`)

---

## 🔒 Security Notice

> **The Run button does NOT compile or execute user-provided C++ code.**

Running arbitrary C++ code inside the GUI process would be a critical security and stability risk. The Run button performs a whitespace-normalised text comparison between the user's code and the stored solution.

**Future enhancement:** Use `QProcess` to pipe code to an external sandboxed `g++` process with resource limits (CPU time, memory), capture stdout, and display real compiler output. This is left as a clearly-labelled future task.

---

## 🔌 Module Integration

Your teammates' modules are not yet available. The file `bugchallengeadapter.cpp` contains **clearly labelled MOCK data** with 9 sample challenges (3 per difficulty).

**To integrate the real bug-generation module:**
1. Ask your teammate for their class/function interface
2. In `bugchallengeadapter.cpp`, replace `loadMockChallenges()` with calls to their API
3. Map their data fields to `BugChallenge` setters

---

## ✅ Implemented Features

- [x] Home screen with title, subtitle, instructions, difficulty selector, Start button
- [x] Debug screen with code editor, description, expected output, output panel
- [x] Run, Hint, Submit, Next Bug, Home buttons
- [x] QStackedWidget navigation (Home ↔ Debug ↔ Result)
- [x] ScoreManager class with guarded deductions
- [x] Countdown timer with QTimer (single-timer guarantee)
- [x] Result screen with score, time, hints, wrongs, rank badge
- [x] Mock challenge adapter with 9 sample challenges (3 per difficulty)
- [x] OOP design: encapsulation, inheritance, composition, constructors, containers

---

## 👤 Team Responsibilities

| Module | Owner |
|---|---|
| GUI, Scoring, Timer | **You** |
| Bug Generation | Teammate A |
| Code Evaluation | Teammate B |

---

*BUGBOX — College Project | Qt 6 Widgets | C++ | CMake*
