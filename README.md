# ducky-snake 🦆

A mini game: Snake but with ducks!

## What it is

This is a duck-themed version of the classic Snake game. Control a snake, represented by a Mother duck (🦆) for the head and children ducks (🐥) and eggs (🥚) for the body, as it moves around a 20x20 grid, collecting vegetables to grow longer and increase your score.
<!--
## Features

- **Classic Snake Gameplay**: Move with W-A-S-D keys (W=up, A=left, S=down, D=right)
- **Terminal Interface**: Uses ASCII characters to display the game in the terminal
- **Real-time Input**: Non-blocking keyboard input so the game runs smoothly
- **Score System**: Each fruit eaten adds 10 points to your score
- **Cross-platform**: Uses POSIX functions for terminal control (works on Unix-like systems)
 -->
## How to Play

- Use **W-A-S-D** keys to control the snake's direction
- Press **X** to quit the game
- Eat a vegetable to get 10 points and an egg. Over time, the eggs will hatch into ducklings.
- Avoid hitting the walls or your own body
- Ducks chain cannot go backward.

## How to Run
Run the file:
```bash
./ducky
```

## Game Controls

- **W** - Up
- **A** - Left
- **S** - Down
- **D** - Right
- **X** - Quit game

## Technical Details

The game uses:
- A vector to store snake body segments
- Terminal manipulation for real-time display and input
- A game loop that handles drawing, input, and game logic
- Random fruit placement after each collection

Enjoy playing ducky-snake! 🦆
