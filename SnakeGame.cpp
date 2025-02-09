#include <iostream> 
#include <cstdlib>  
#include <ctime>    
#include <conio.h>  

using namespace std;

#define ROWS 20
#define COLS 20
#define MAX_SIZE 400

#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'

void delay(int milliseconds) {
    clock_t start_time = clock();
    while (clock() < start_time + milliseconds * CLOCKS_PER_SEC / 1000);
}

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

class Snake {
private:
    int body[MAX_SIZE][2]; 
    int length;
    char direction;

public:
    Snake(int startX, int startY) {
        body[0][0] = startX; 
        body[0][1] = startY; 
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

class Game {
private:
    Snake snake;
    Food food;
    bool gameOver;
    int speed;

public:
    Game(int difficulty) : snake(ROWS / 2, COLS / 2), food() {
        gameOver = false;
        if (difficulty == 1) speed = 300;   
        else if (difficulty == 2) speed = 200; 
        else speed = 100;  
    }

    void drawGrid() {
        system("cls");

        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (gameOver && i >= 8 && i <= 11 && j >= 5 && j <= 14) {
                    const char *gameOverText = "GAME OVER";
                    cout << "\033[31m" << gameOverText[j - 5] << " \033[0m"; 
                }
                else if (food.x == i && food.y == j) {
                    cout << "\033[33mF \033[0m"; 
                }
                else {
                    bool isSnake = false;
                    for (int k = 0; k < snake.getLength(); k++) {
                        if (snake.getBodyX(k) == i && snake.getBodyY(k) == j) {
                            cout << "\033[92mO \033[0m"; 
                            isSnake = true;
                            break;
                        }
                    }
                    if (!isSnake) {
                        cout << "\033[34m. \033[0m"; 
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
