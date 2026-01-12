#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "Board.h"
#include "Tetromino.h"

class Renderer {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    int blockSize;
    int boardX, boardY;  // Top-left position of board on screen
    int screenWidth, screenHeight;

    // Colors for each tetromino type
    SDL_Color pieceColors[7];
    SDL_Color backgroundColor;
    SDL_Color borderColor;
    SDL_Color gridColor;

    // Animation state
    int lineClearAnimFrame;
    std::vector<int> linesToClear;

public:
    Renderer(int screenWidth = 800, int screenHeight = 600);
    ~Renderer();

    void init();
    void clear();

    // Main rendering methods
    void renderGame(const Board& board, const Tetromino& currentPiece,
                    const Tetromino& nextPiece, int score, int highScore, int level, int lines,
                    const Tetromino* ghostPiece = nullptr,
                    const Tetromino* holdPiece = nullptr,
                    bool canHold = true);
    void renderGameOver(int score, int highScore, int level, int lines);
    void renderPauseScreen();
    void renderTitleScreen();
    void present();
    bool isRunning() const;

    // Line clear animation
    void startLineClearAnimation(const std::vector<int>& lines);
    bool updateLineClearAnimation();  // Returns true if animation is still playing

    // Utility
    void renderRect(int x, int y, int w, int h, SDL_Color color);
    void renderBlock(int gridX, int gridY, SDL_Color color, bool isGhost = false);
    void renderBlockAt(int x, int y, int size, SDL_Color color, bool isGhost = false);
    void renderText(const std::string& text, int x, int y, SDL_Color color, int scale = 2);
    std::string formatNumber(int number);
};

#endif
