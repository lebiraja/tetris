#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "Tetromino.h"

class Board {
private:
    static const int WIDTH = 10;
    static const int HEIGHT = 20;
    
    // Game board: -1 = empty, 0-6 = tetromino type
    std::vector<std::vector<int>> grid;
    
public:
    Board();
    
    // Check if piece can be placed at position
    bool canPlace(const Tetromino& piece) const;
    
    // Place piece on board (finalize it)
    void place(const Tetromino& piece);
    
    // Clear completed lines and return count
    int clearLines();
    
    // Check if board is full (game over)
    bool isGameOver() const;
    
    // Get cell value
    int getCell(int x, int y) const;
    
    // Reset board
    void clear();
    
    // Get dimensions
    int getWidth() const { return WIDTH; }
    int getHeight() const { return HEIGHT; }
    
    // Get board for rendering
    const std::vector<std::vector<int>>& getGrid() const { return grid; }
};

#endif
