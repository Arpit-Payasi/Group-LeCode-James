# Group-LeCode-James
# Snake Game

This is a simple console-based Snake game implemented in C++ where you control a snake to collect food and grow in size. The game ends when the snake collides with the wall or itself.

## Features

- Grid-based gameplay (20x20 grid)
- Snake movement with WASD keys
- Snake grows as it eats food
- Three difficulty levels: Easy, Medium, Hard
- Simple text-based game over message
- Grid with color-coded elements (Snake, Food, and Grid Cells)

## Game Controls

- **w**: Move up
- **s**: Move down
- **a**: Move left
- **d**: Move right

## How to Run

1. Ensure you have a C++ compiler (like GCC) installed on your machine.
2. Copy the code into a `.cpp` file (e.g., `snake_game.cpp`).
3. Compile the code:
   ```bash
   g++ snake_game.cpp -o snake_game

## 𝐃𝐢𝐟𝐟𝐢𝐜𝐮𝐥𝐭𝐲 𝐋𝐞𝐯𝐞𝐥𝐬
Easy: Slower speed, more time to react.
Medium: Moderate speed.
Hard: Fast speed, more challenging.

License

This project is open source and free to use. Feel free to or modify it.


## 𝐂ode
#include <iostream>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include <conio.h>  // For _kbhit() and _getch() (Windows only)

using namespace std;

// Grid size
#define ROWS 20
#define COLS 20
#define MAX_SIZE 400 // Max snake length

// Directions
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'

// Function to create a delay without using sleep functions
void delay(int milliseconds) {
    clock_t start_time = clock();
    while (clock() < start_time + milliseconds * CLOCKS_PER_SEC / 1000);
}

// Class to manage food
class Food {
public:
    int x, y;

    Food() {
        generate();
    }

    void generate() {
        x = rand() % ROWS;
        y = rand() % COLS;
    }
};

// Snake class using an array instead of vector
class Snake {
private:
    int body[MAX_SIZE][2]; // Stores snake positions (row, col)
    int length;
    char direction;

public:
    Snake(int startX, int startY) {
        body[0][0] = startX; // Head X
        body[0][1] = startY; // Head Y
        length = 1;
        direction = RIGHT;
    }

    void move() {
        for (int i = length - 1; i > 0; i--) {
            body[i][0] = body[i - 1][0];
            body[i][1] = body[i - 1][1];
        }

        if (direction == UP) body[0][0]--;
        if (direction == DOWN) body[0][0]++;
        if (direction == LEFT) body[0][1]--;
        if (direction == RIGHT) body[0][1]++;
    }

    void grow() {
        if (length < MAX_SIZE) {
            body[length][0] = body[length - 1][0];
            body[length][1] = body[length - 1][1];
            length++;
        }
    }

    bool hasCollided() {
        int headX = body[0][0];
        int headY = body[0][1];

        if (headX < 0 || headX >= ROWS || headY < 0 || headY >= COLS) {
            return true;
        }

        for (int i = 1; i < length; i++) {
            if (body[i][0] == headX && body[i][1] == headY) {
                return true;
            }
        }

        return false;
    }

    void setDirection(char newDirection) {
        if ((newDirection == UP && direction != DOWN) ||
            (newDirection == DOWN && direction != UP) ||
            (newDirection == LEFT && direction != RIGHT) ||
            (newDirection == RIGHT && direction != LEFT)) {
            direction = newDirection;
        }
    }

    int getHeadX() { return body[0][0]; }
    int getHeadY() { return body[0][1]; }
    int getLength() { return length; }
    int getBodyX(int i) { return body[i][0]; }
    int getBodyY(int i) { return body[i][1]; }
};

// Game class to control game logic
class Game {
private:
    Snake snake;
    Food food;
    bool gameOver;
    int speed;

public:
    Game(int difficulty) : snake(ROWS / 2, COLS / 2), food() {
        gameOver = false;
        if (difficulty == 1) speed = 300;   // Easy
        else if (difficulty == 2) speed = 200; // Medium
        else speed = 100;  // Hard
    }

    void drawGrid() {
        system("cls"); // "cls" for Windows, "clear" for Linux/macOS

        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (gameOver && i >= 8 && i <= 11 && j >= 5 && j <= 14) {
                    const char *gameOverText = "GAME OVER";
                    cout << "\033[31m" << gameOverText[j - 5] << " \033[0m"; // Red "GAME OVER"
                }
                else if (food.x == i && food.y == j) {
                    cout << "\033[33mF \033[0m"; // Yellow for food
                }
                else {
                    bool isSnake = false;
                    for (int k = 0; k < snake.getLength(); k++) {
                        if (snake.getBodyX(k) == i && snake.getBodyY(k) == j) {
                            cout << "\033[92mO \033[0m"; // Neon green for snake
                            isSnake = true;
                            break;
                        }
                    }
                    if (!isSnake) {
                        cout << "\033[34m. \033[0m"; // Blue for grid cells
                    }
                }
            }
            cout << endl;
        }
    }

    void getInput() {
        if (_kbhit()) {
            char key = _getch();
            snake.setDirection(key);
        }
    }

    void update() {
        snake.move();

        if (snake.hasCollided()) {
            gameOver = true;
        }

        if (snake.getHeadX() == food.x && snake.getHeadY() == food.y) {
            snake.grow();
            food.generate();
        }
    }

    void run() {
        while (!gameOver) {
            drawGrid();
            getInput();
            update();
            delay(speed);
        }

        drawGrid();
        cout << "\nGame Over! Your score: " << snake.getLength() - 1 << endl;
    }
};

int main() {
    srand(time(0));

    int difficulty;
    cout << "\nSelect Difficulty Level: \n";
    cout << "1. Easy\n2. Medium\n3. Hard\n";
    cin >> difficulty;

    Game game(difficulty);
    game.run();
    return 0;
}


## 𝐀𝐮𝐭𝐡𝐨𝐫𝐬
Arpit Payasi
Chetan Raghav
Kush Solanki
Nishta Vadher

