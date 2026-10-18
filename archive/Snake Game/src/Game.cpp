#include "Game.hpp"

Game::Game(Console& console) noexcept 
: console(console){
    setup();
}

void Game::run() noexcept {
    draw();
    input();
    logic();
    Sleep(100);
}

void Game::restart() noexcept{
    if (console.getStatus()){
        setup();
    }
    
}
void Game::setup() noexcept {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    tailX[0] = x;
    tailY[0] = y;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
}

void Game::draw() noexcept  {
    
    console.clear();

    std::string line = "";

    line = line + "Use WASD to control. Press X to exit.\n";
    for (int i = 0; i < width + 2; i++){
        line = line + "#";      
    }
        
    line = line + '\n';

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width + 1; j++) {
            if (j == 0 || j == width)
                line = line + "#";
            if (i == y && j == x)
                line = line + "O";
            else if (i == fruitY && j == fruitX)
                line = line + "F";
            else {
                bool print = false;
                for (uint32_t k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        line = line + "O";
                        print = true;
                    }
                }
                if (!print && j < width)
                    line = line + " ";
            }
        }
        line = line + '\n';
    }

    for (int i = 0; i < width + 2; i++){
        line = line + "#";
    }
    line = line + '\n';

    line = line + "Score: " + std::to_string(score);

    std::cout << line << std::endl;
}

void Game::input() noexcept  {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            if (dir != RIGHT)
            {
                dir = LEFT;
            }
            break;
        case 'd':
            if (dir != LEFT)
            {
                dir = RIGHT;    
            }
            break;
        case 'w':
            if (dir != DOWN)
            {
                dir = UP;
            }
            break;
        case 's':
            if (dir != UP)
            {
                dir = DOWN;
            }
            break;
        case 'x':
            gameOver = true;
            break;
        }
    }
}

void Game::logic() noexcept  {
    int prevX = tailX[0];
    int prevY = tailY[0];
    tailX[0] = x;
    tailY[0] = y;

    for (uint32_t i = 1; i < nTail; i++) {
        tailX[i] = tailX[i] ^ prevX; 
        prevX = tailX[i] ^ prevX; 
        tailX[i] = tailX[i] ^ prevX; 

        tailY[i] = tailY[i] ^ prevY; 
        prevY = tailY[i] ^ prevY; 
        tailY[i] = tailY[i] ^ prevY; 
    } 

    switch (dir) {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    if (x >= width || x < 0 || y >= height ||  y < 0) {
        gameOver = true;
    }

    for (uint32_t i = 1; i < nTail; i++){
        if (tailX[i] == x && tailY[i] == y){
            gameOver = true; 
        }
    }

    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

bool Game::isGameOver() noexcept 
{
    return gameOver;
}



uint16_t Game::getScore() noexcept 
{
    return score;
}