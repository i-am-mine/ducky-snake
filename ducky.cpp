#include <iostream>
#include <vector>
#include <cstdlib>
#include <unistd.h>   // for usleep
#include <termios.h>
#include <fcntl.h>
#include <iomanip>
#include <chrono>
#include <cmath>
using namespace std;

// ANSI color codes for better UI
const string RESET = "\033[0m";
const string BOLD = "\033[1m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";
const string WHITE = "\033[37m";

bool gameOver;
const int width = 10;  // Reduced width to accommodate emojis
const int height = 10; // Reduced height to accommodate emojis
int x, y, fruitX, fruitY, score;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;
vector<pair<int,int>> snake;

// Animation and timing variables
float animTime = 0.0f;
const float ANIM_SPEED = 0.1f;
const int TARGET_FPS = 30;
const int FRAME_DURATION = 1000000 / TARGET_FPS; // microseconds per frame
const int GAME_SPEED = 500000; // 500ms between moves (half a second)
auto lastTime = chrono::high_resolution_clock::now();
auto lastMoveTime = chrono::high_resolution_clock::now();

// Smooth movement variables
float smoothX, smoothY;
float targetX, targetY;
bool isMoving = false;

// Cross-platform _kbhit
int kbhit() {
    termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF){
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

char getch() {
    char c;
    cin >> c;
    return c;
}

void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    snake.clear();
    snake.push_back({x, y});
    
    // Initialize smooth movement
    smoothX = x;
    smoothY = y;
    targetX = x;
    targetY = y;
    isMoving = false;
    animTime = 0.0f;
    lastMoveTime = chrono::high_resolution_clock::now();
}

void Draw() {
    system("clear"); // clear terminal
    
    // Animated title with blinking effect
    if (int(animTime * 2) % 2 == 0) {
        cout << BOLD << CYAN << "🦆 DUCKY SNAKE 🦆" << RESET << endl;
    } else {
        cout << BOLD << YELLOW << "🦆 DUCKY SNAKE 🦆" << RESET << endl;
    }
    cout << string(24, '=') << endl << endl;
    
    // Top border with animated corners
    cout << BLUE << "┌";
    for (int i = 0; i < width; i++) cout << "──";
    cout << "┐" << RESET << endl;

    for (int i = 0; i < height; i++) {
        cout << BLUE << "│" << RESET;
        for (int j = 0; j < width; j++) {
            // Smooth duck head with animation
            if (i == y && j == x) {
                // Animated duck head with blinking
                if (int(animTime * 4) % 2 == 0) {
                    cout << BOLD << YELLOW << "🦆" << RESET; // Eyes open
                } else {
                    cout << BOLD << YELLOW << "🦆" << RESET; // Eyes closed (same for now)
                }
            }
            // Animated fruit with pulsing effect
            else if (i == fruitY && j == fruitX) {
                if (int(animTime * 3) % 2 == 0) {
                    cout << BOLD << RED << "🍎" << RESET; // Normal size
                } else {
                    cout << BOLD << RED << "🍎" << RESET; // Slightly larger (same for now)
                }
            }
            else {
                bool print = false;
                for (int k = 1; k < snake.size(); k++) {
                    if (snake[k].first == j && snake[k].second == i) {
                        // Animated ducklings with wiggling effect
                        if (int(animTime * 5 + k) % 2 == 0) {
                            cout << YELLOW << "🐥" << RESET; // Normal
                        } else {
                            cout << YELLOW << "🐥" << RESET; // Wiggling (same for now)
                        }
                        print = true;
                    }
                }
                if (!print) cout << "  "; // Double space for emoji alignment
            }
        }
        cout << BLUE << "│" << RESET << endl;
    }

    // Bottom border
    cout << BLUE << "└";
    for (int i = 0; i < width; i++) cout << "──";
    cout << "┘" << RESET << endl << endl;
    
    // Enhanced game info with animations
    cout << BOLD << "Score: " << YELLOW << score << RESET;
    if (score > 0 && int(animTime * 2) % 2 == 0) {
        cout << BOLD << GREEN << " ⭐" << RESET; // Sparkle effect for score
    }
    cout << endl;
    
    cout << "Duck Chain: " << GREEN << snake.size() << " ducks" << RESET;
    if (snake.size() > 1) {
        cout << " " << YELLOW;
        for (int i = 0; i < min(3, (int)snake.size()-1); i++) {
            cout << "🐥";
        }
        cout << RESET;
    }
    cout << endl << endl;
    
    // Enhanced controls with better formatting
    cout << CYAN << "Controls:" << RESET << endl;
    cout << "  " << WHITE << "W" << RESET << " - Up    " << WHITE << "A" << RESET << " - Left" << endl;
    cout << "  " << WHITE << "S" << RESET << " - Down  " << WHITE << "D" << RESET << " - Right" << endl;
    cout << "  " << WHITE << "X" << RESET << " - Quit" << endl;
}

void Input() {
    if (kbhit()) {
        char c = getchar();
        switch (c) {
            case 'a': 
            case 'A': dir = LEFT; break;
            case 'd': 
            case 'D': dir = RIGHT; break;
            case 'w': 
            case 'W': dir = UP; break;
            case 's': 
            case 'S': dir = DOWN; break;
            case 'x': 
            case 'X': gameOver = true; break;
        }
    }
}

void Logic() {
    // Update animation time
    animTime += ANIM_SPEED;
    
    // Check if it's time to move the snake
    auto currentTime = chrono::high_resolution_clock::now();
    auto timeSinceLastMove = chrono::duration_cast<chrono::microseconds>(currentTime - lastMoveTime).count();
    
    if (timeSinceLastMove >= GAME_SPEED && dir != STOP) {
        targetX = x;
        targetY = y;
        
        switch (dir) {
            case LEFT:  targetX = x - 1; break;
            case RIGHT: targetX = x + 1; break;
            case UP:    targetY = y - 1; break;
            case DOWN:  targetY = y + 1; break;
            default: break;
        }
        
        // Check boundaries before moving
        if (targetX >= 0 && targetX < width && targetY >= 0 && targetY < height) {
            // Check collision with snake body
            bool collision = false;
            for (int i = 1; i < snake.size(); i++) {
                if (snake[i].first == targetX && snake[i].second == targetY) {
                    collision = true;
                    break;
                }
            }
            
            if (!collision) {
                // Move snake body
                int prevX = snake[0].first;
                int prevY = snake[0].second;
                int prev2X, prev2Y;
                snake[0] = {x, y};
                for (int i = 1; i < snake.size(); i++) {
                    prev2X = snake[i].first;
                    prev2Y = snake[i].second;
                    snake[i].first = prevX;
                    snake[i].second = prevY;
                    prevX = prev2X;
                    prevY = prev2Y;
                }
                
                // Update position
                x = targetX;
                y = targetY;
                
                // Check fruit collection
                if (x == fruitX && y == fruitY) {
                    score += 10;
                    fruitX = rand() % width;
                    fruitY = rand() % height;
                    snake.push_back({x, y});
                }
                
                // Update last move time
                lastMoveTime = currentTime;
            } else {
                gameOver = true;
            }
        } else {
            gameOver = true;
        }
    }
}

void ShowGameOver() {
    system("clear");
    cout << BOLD << RED << "╔══════════════════════════════╗" << RESET << endl;
    cout << BOLD << RED << "║        GAME OVER! 🦆        ║" << RESET << endl;
    cout << BOLD << RED << "╚══════════════════════════════╝" << RESET << endl << endl;
    
    cout << BOLD << "Final Score: " << YELLOW << score << RESET << endl;
    cout << "Duck Chain Length: " << GREEN << snake.size() << RESET << endl << endl;
    
    cout << CYAN << "Press 'R' to restart or 'Q' to quit" << RESET << endl;
    
    // Clear any remaining input
    while (kbhit()) getchar();
    
    char choice;
    while (true) {
        if (kbhit()) {
            choice = getchar();
            if (choice == 'r' || choice == 'R') {
                Setup();
                return;
            } else if (choice == 'q' || choice == 'Q') {
                cout << BOLD << GREEN << "Thanks for playing Ducky Snake! 🦆" << RESET << endl;
                exit(0);
            }
        }
        usleep(100000);
    }
}

int main() {
    cout << BOLD << CYAN << "Welcome to Ducky Snake! 🦆" << RESET << endl;
    cout << "Press any key to start..." << endl;
    getchar();
    
    Setup();
    
    // Main game loop with proper timing
    while (!gameOver) {
        auto currentTime = chrono::high_resolution_clock::now();
        auto deltaTime = chrono::duration_cast<chrono::microseconds>(currentTime - lastTime).count();
        
        if (deltaTime >= FRAME_DURATION) {
            Draw();
            Input();
            Logic();
            lastTime = currentTime;
        }
        
        // Small sleep to prevent excessive CPU usage
        usleep(1000);
    }
    
    ShowGameOver();
    
    // Restart loop with proper timing
    while (true) {
        Setup();
        while (!gameOver) {
            auto currentTime = chrono::high_resolution_clock::now();
            auto deltaTime = chrono::duration_cast<chrono::microseconds>(currentTime - lastTime).count();
            
            if (deltaTime >= FRAME_DURATION) {
                Draw();
                Input();
                Logic();
                lastTime = currentTime;
            }
            
            usleep(1000);
        }
        ShowGameOver();
    }
    
    return 0;
}
