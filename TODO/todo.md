# Task to complete

## Versions
- Ver1.0: console gaming, used for technical guys. Basic, compilation requirements
- Ver2.0: with Qt framework, used for techinical guys specializing in C++. More complex, Qt installation requirements
- Ver3.0: webapp, used for all users, non-technical including.


## Graphic
- Upgrade the graphic from TUI to GUI
    - Study Qt framework ?
    - Qt: design OOP for the game

## Feature
- Create, or integrate, a random food location generator
    - Currently, the food randomness is done with `rand() % size of gameframe`
    - Idea: use Procedural Generation?
- Mother duck -> Egg -> Ducklings
- Beside food, insert obstacles: poisonous mushrooms, cactus ...
- Set up the game levels
- Replace control buttons W A S D with arrow buttons ?

## Bug
- When the user controls the ducks in the opposite direction, the display fails. Logically, the ducklings are led by the mother duck, so the reverse movement should not happen.
