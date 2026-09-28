# 🕹️ Terminal Pac-Man — Procedural 2D Game Engine in C

[![C11](https://img.shields.io/badge/Language-C11-blue.svg?logo=c)](https://en.cppreference.com/w/c/11)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Build Status](https://github.com/jyliew1912/YOUR_REPO_NAME/actions/workflows/ci.yml/badge.svg)](https://github.com/jyliew1912/YOUR_REPO_NAME/actions/workflows/ci.yml)

A lightweight, terminal-based grid navigation game implemented in **C11**. The project showcases procedural grid generation, dynamic 2D array allocation via heap double pointers, boundary collision validation, and turn-based entity updates without external dependencies.


## 📽️ Gameplay Demonstration

https://github.com/user-attachments/assets/5c51cd72-eef2-495d-bedd-c99108746e5e


## 🎮 Game Mechanics & Entity Systems

The game initializes a customizable $N \times M$ grid ($N, M \ge 5$) with toroidal boundary wrapping and dynamic entity placement:

### Grid Elements
| Token | Entity | Behavior & Rules |
| :---: | :--- | :--- |
| `P` | **Player (Pac-Man)** | Controlled via `WASD` keys. Navigates the board to collect scores and items. |
| `S` | **Score Pellets** | Primary objective. Consuming all pellets triggers victory and advances to the next stage. |
| `M` | **Monsters** | Autonomous adversaries stepping randomly after each player move. Contact reduces player lives. |
| `$` | **Revive Coins** | Special currency; collecting 2 coins allows the player to purchase an extra life upon defeat. |
| `X` | **Static Obstacles** | Solid wall blocks appearing in higher difficulty tiers that block movement. |

### Dynamic Booster Effects (`B`)
Whenever Pac-Man consumes a Booster item (`B`), one of five outcomes is triggered randomly:
- ❄️ **Freeze Booster:** Freezes all monster movements for 3 consecutive player turns.
- 🧹 **Less Monster Booster:** Permanently removes one monster from the board (+3 bonus points).
- ❤️ **Extra Life Booster:** Increases total remaining lives by 1 (+3 bonus points).
- ⚠️ **Hazard Booster:** Spawns an additional obstacle (`X`) or monster (`M`) (-3 penalty points).
- ⭐ **Score Surge:** Grants a flat +5 point score bonus.


## 🏛️ Technical Architecture & Implementation

### 1. Heap-Allocated 2D Grid Architecture
- The game board is managed dynamically through a double pointer (`char**`).
- **Dynamic Allocation (`allocate()`):** Allocates memory based on runtime dimensions entered by the user, avoiding static compile-time array limits.
- **Teardown & Memory Safety (`deallocate()`):** Systematically frees nested row buffers before releasing the row pointer array, preventing memory leaks between successive runs.

### 2. Turn Engine & State Updates
- **Toroidal Topology (`moveRow`, `moveColumn`):** Implements smooth wrapping across grid perimeters (moving past the upper boundary wraps to the bottom edge, and vice-versa).
- **Collision & Occupancy Detection:** Pre-validates intended coordinate hops to prevent adversaries from overwriting existing score pellets (`S`) or impassable blocks (`X`).
- **Comprehensive Score Formulation (`arrayStoring()`):** Calculates composite player performance balancing pellets eaten, board area, and movement efficiency:
  $$\text{Score} = 3 \cdot \text{Points} + 2 \cdot (\text{Length} \times \text{Width}) + \text{Level} - \text{Moves}$$


## 🚀 Compilation & Execution

### Prerequisites
- GCC (`gcc >= 9.0`) or Clang
- GNU Make (Optional)

### 1. Build the Binary

Using standard `make`:
```bash
make
```

Or compiling directly with `gcc`:

```bash
gcc -Wall -Wextra -std=c11 -O2 pacman.c -o pacman -lm
```

### 2. Run the Game

**On Linux / macOS:**

```bash
./pacman
```

**On Windows (PowerShell / Command Prompt):**

```powershell
.\pacman.exe
```


## 🕹️ Controls & Navigation

| Key | Direction / Action |
| --- | --- |
| `w` | Move Up|
| `s` | Move Down|
| `a` | Move Left|
| `d` | Move Right|


## 🧠 Core Engineering Takeaways

* **Low-Level Memory Hygiene:** Gained hands-on experience structuring, allocating, and cleanly deallocating dynamic 2D pointers in heap memory.
* **Deterministic Game Loops:** Built a modular state machine handling entity state changes, turn sequencing, and user input validation.
* **Array Mutation & Tracking:** Designed dynamic scanning and coordinate tracking to manipulate and scramble active entities without corruption.


## 📄 License

This project is open-source and distributed under the [MIT License](https://www.google.com/search?q=LICENSE&utm_source=gemini).

```

```
