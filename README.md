# ducky-snake 🦆

A mini game: Snake but with ducks! A terminal-based implementation of the classic Snake game written in C++.

## What it is

This is a duck-themed version of the classic Snake game that runs in your terminal. Control a snake (represented by "O" for the head and "o" for the body) as it moves around a 20x20 grid, collecting fruit to grow longer and increase your score.

## Features

- **Classic Snake Gameplay**: Move with WASD keys (w=up, a=left, s=down, d=right)
- **Terminal Interface**: Uses ASCII characters to display the game in the terminal
- **Real-time Input**: Non-blocking keyboard input so the game runs smoothly
- **Score System**: Each fruit eaten adds 10 points to your score
- **Cross-platform**: Uses POSIX functions for terminal control (works on Unix-like systems)

## How to Play

- Use **WASD** keys to control the snake's direction
- Press **X** to quit the game
- Eat the fruit ("F") to grow longer and increase your score
- Avoid hitting the walls or your own body

## How to Run

### Prerequisites
- A Unix-like system (macOS, Linux, etc.)
- A C++ compiler (g++, clang++, etc.)

### Compilation and Execution

1. Compile the game:
   ```bash
   g++ -o ducky-snake ducky.cpp
   ```

2. Run the game:
   ```bash
   ./ducky-snake
   ```

### Alternative compilation
If you prefer clang++:
```bash
clang++ -o ducky-snake ducky.cpp
```

## Game Controls

- **W** - Move up
- **A** - Move left  
- **S** - Move down
- **D** - Move right
- **X** - Quit game

## Technical Details

The game uses:
- A vector to store snake body segments
- Terminal manipulation for real-time display and input
- A game loop that handles drawing, input, and game logic
- Random fruit placement after each collection

Enjoy playing ducky-snake! 🦆
