# Pac-Man (C Terminal Game)

A terminal-based Pac-Man style game implemented in **C**.

This project was developed as the **final project for the course "Program Design I"** and demonstrates core C programming concepts such as pointers, dynamic memory allocation, and 2D array manipulation.

## Game Overview

The game generates a dynamic 2D map where the player controls **Pac-Man (P)**.

After each player move, monsters also move randomly across the map. If the player encounters a monster, the game ends.

## Features

- Customizable map size (minimum 5×5)
- Randomly generated map elements
- Monster AI with random movement
- Booster system including:
- Freeze booster
- Less monster booster
- Extra life booster
- Dynamic map allocation using `malloc`
- Pointer-based implementation

## How to Run

Compile the program:
```
gcc pacman.c -o pacman
```

Run the game:

```
./pacman
```

Follow the instructions in the terminal to start the game.

## What I Learned

Through this project I practiced:

- Designing a complete terminal-based game in **C**
- Working with **pointers and dynamic memory allocation**
- Managing 2D arrays for **game maps**
- Implementing a **game loop and event handling**
- Using **random number** generation for gameplay mechanics
- Structuring a larger program with **multiple functions**

## Demo

https://github.com/user-attachments/assets/5c51cd72-eef2-495d-bedd-c99108746e5e

## Course Context

Course: **Program Design I**  
Language: **C**  
Completed: **2023**

The full assignment specification for this project was provided by the course instructor: [Project specification](https://docs.google.com/document/d/1pbmqKgjQ75Juk1BeeBKcBVxS9AebBml3H2rgWHhogiw/edit?usp=sharing)

