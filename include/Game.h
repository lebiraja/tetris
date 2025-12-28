#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Tetromino.h"
#include "Renderer.h"
#include <memory>

class Game {
private:
    Board board;
    Tetromino currentPiece;
    Tetromino nextPiece;
    std::unique_ptr<Renderer> renderer;
    
    int score;
    int highScore;
    int level;
    int lines;
    int frameCounter;
    bool gameOver;
    bool paused;
    bool running;
    
    // Game speed (frames until piece moves down)
    int dropSpeed;
    
public:
    Game();
    
    // Game loop methods
    void init();
    void handleInput();
    void update();
    void render();
    void run();
    
    // Game state
    bool isGameOver() const { return gameOver; }
    bool isPaused() const { return paused; }
    bool isRunning() const { return running; }
    
    int getScore() const { return score; }
    int getHighScore() const { return highScore; }
    int getLevel() const { return level; }
    int getLines() const { return lines; }
    
    // Tetromino methods
    void spawnNewPiece();
    bool movePieceDown();
    bool movePieceLeft();
    bool movePieceRight();
    bool rotatePiece();
    
private:
    void lockPiece();
    void increaseLevel();
    void updateDropSpeed();
    void saveHighScore();
    void loadHighScore();
    void resetGame();
};

#endif
