#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
using namespace std;

const int WIDTH = 10;
const int HEIGHT = 20;

char TETROMINOS[7][4][4] = {
    {
        {' ', ' ', ' ', ' '},
        {'#', '#', '#', '#'},
        {' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' '}
    },
    {
        {'#', '#', ' ', ' '},
        {'#', '#', ' ', ' '},
        {' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' '}
    },
    {
        {' ', '#', ' ', ' '},
        {'#', '#', '#', ' '},
        {' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' '}
    },
    {
        {' ', '#', '#', ' '},
        {'#', '#', ' ', ' '},
        {' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' '}
    },
    {
        {'#', '#', ' ', ' '},
        {' ', '#', '#', ' '},
        {' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' '}
    },
    {
        {'#', ' ', ' ', ' '},
        {'#', '#', '#', ' '},
        {' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' '}
    },
    {
        {' ', ' ', '#', ' '},
        {'#', '#', '#', ' '},
        {' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' '}
    }
};

const char* colors[7] = {
    "\033[36m",
    "\033[33m",
    "\033[35m",
    "\033[32m",
    "\033[31m",
    "\033[34m",
    "\033[91m"
};

class Tetromino {
public:
    char shape[4][4];
    int x, y, type;

    Tetromino() {
        x = WIDTH / 2 - 2;
        y = 0;
        type = rand() % 7;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = TETROMINOS[type][i][j];
    }

    void rotate() {
        char rotated[4][4];
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                rotated[j][3 - i] = shape[i][j];
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = rotated[i][j];
    }
};

class Board {
public:
    char grid[HEIGHT][WIDTH] = {};
    int colorGrid[HEIGHT][WIDTH] = {};
    int score = 0;

    Board() {
        for (int i = 0; i < HEIGHT; i++)
            for (int j = 0; j < WIDTH; j++) {
                grid[i][j] = ' ';
                colorGrid[i][j] = -1;
            }
    }

    void draw(Tetromino &tetro) {
        system("cls");

        for (int i = 0; i < HEIGHT; i++) {
            cout << "|";
            for (int j = 0; j < WIDTH; j++) {
                bool isTetromino = false;

                for (int ti = 0; ti < 4; ti++)
                    for (int tj = 0; tj < 4; tj++)
                        if (tetro.shape[ti][tj] == '#' && tetro.y + ti == i && tetro.x + tj == j)
                            isTetromino = true;

                if (isTetromino)
                    cout << colors[tetro.type] << "#" << "\033[0m";
                else if (grid[i][j] == '#')
                    cout << colors[colorGrid[i][j]] << "#" << "\033[0m";
                else
                    cout << " ";
            }
            cout << "|" << endl;
        }
        cout << string(WIDTH + 2, '-') << endl;
        cout << "Score: \033[33m" << score << "\033[0m" << endl;
    }

    bool isValidMove(Tetromino &tetro, int newX, int newY) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                if (tetro.shape[i][j] == '#' &&
                    (newY + i >= HEIGHT || newX + j < 0 || newX + j >= WIDTH || grid[newY + i][newX + j] == '#'))
                    return false;
        return true;
    }

    void placeTetromino(Tetromino &tetro) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                if (tetro.shape[i][j] == '#') {
                    grid[tetro.y + i][tetro.x + j] = '#';
                    colorGrid[tetro.y + i][tetro.x + j] = tetro.type;
                }
    }

    void clearLines() {
        int linesCleared = 0;
        for (int i = HEIGHT - 1; i >= 0; i--) {
            bool full = true;
            for (int j = 0; j < WIDTH; j++)
                if (grid[i][j] == ' ') {
                    full = false;
                    break;
                }

            if (full) {
                linesCleared++;
                for (int k = i; k > 0; k--) {
                    for (int j = 0; j < WIDTH; j++) {
                        grid[k][j] = grid[k - 1][j];
                        colorGrid[k][j] = colorGrid[k - 1][j];
                    }
                }

                for (int j = 0; j < WIDTH; j++) {
                    grid[0][j] = ' ';
                    colorGrid[0][j] = -1;
                }
                i++;
            }
        }
        score += linesCleared * 100;
    }
};

class Game {
    Board board;
    Tetromino tetro;
    bool gameOver;
    int speed;

public:
    Game() {
        gameOver = false;
        speed = 300;
    }

    void updateSpeed() {
        if (board.score > 0) {
            speed = max(50, 500 - (board.score / 200) * 50);
        }
    }

    void run() {
        while (!gameOver) {
            if (_kbhit()) {
                char ch = _getch();
                if (ch == 27) {
                    gameOver = true;
                    break;
                }
                if (ch == ' ') {
                    while (board.isValidMove(tetro, tetro.x, tetro.y + 1))
                        tetro.y++;
                }
                if (ch == -32) {
                    ch = _getch();
                    if (ch == 75 && board.isValidMove(tetro, tetro.x - 1, tetro.y))
                        tetro.x--;
                    else if (ch == 77 && board.isValidMove(tetro, tetro.x + 1, tetro.y))
                        tetro.x++;
                    else if (ch == 80 && board.isValidMove(tetro, tetro.x, tetro.y + 1))
                        tetro.y++;
                    else if (ch == 72) {
                        tetro.rotate();
                        if (!board.isValidMove(tetro, tetro.x, tetro.y))
                            tetro.rotate();
                    }
                }
            }

            if (board.isValidMove(tetro, tetro.x, tetro.y + 1)) {
                tetro.y++;
            } else {
                board.placeTetromino(tetro);
                board.clearLines();
                updateSpeed();
                tetro = Tetromino();

                if (!board.isValidMove(tetro, tetro.x, tetro.y)) {
                    gameOver = true;
                }
            }

            board.draw(tetro);
            Sleep(speed);
        }

        system("cls");
        cout << "\n\n\033[31m===== GAME OVER =====\033[0m\n";
        cout << "Final Score: " << board.score << "\n\n";
    }
};

int main() {
    srand(time(0));
     system("cls");
     cout << "\n\n\033[32m===== WELCOME TO TETRIS =====\033[0m\n";
     cout << "Controls:\n";
     cout << "  Left Arrow  - Move Left\n";
     cout << "  Right Arrow - Move Right\n";
     cout << "  Down Arrow  - Move Down\n";
     cout << "  Up Arrow    - Rotate\n";
     cout << "  Space       - Drop Instantly\n";
     cout << "  ESC         - Quit\n";
     cout << "\n\033[33mPress ENTER to start the game...\033[0m";
     
     while (_getch() != 13);
    Game game;
    game.run();
    return 0;
}
