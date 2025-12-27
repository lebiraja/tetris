#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <string>
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
    
public:
    Renderer(int screenWidth = 800, int screenHeight = 600);
    ~Renderer();
    
    void init();
    void clear();
    void renderGame(const Board& board, const Tetromino& currentPiece, 
                    const Tetromino& nextPiece, int score, int level, int lines);
    void renderGameOver(int score, int level, int lines);
    void present();
    bool isRunning() const;
    
    // Utility
    void renderRect(int x, int y, int w, int h, SDL_Color color);
    void renderBlock(int gridX, int gridY, SDL_Color color);
    void renderText(const std::string& text, int x, int y, SDL_Color color);
};

#endif
