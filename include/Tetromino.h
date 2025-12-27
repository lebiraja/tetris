#ifndef TETROMINO_H
#define TETROMINO_H

#include <vector>

// Tetromino types
enum TetrominoType {
    I = 0,  // Light blue
    O = 1,  // Yellow
    T = 2,  // Purple
    S = 3,  // Green
    Z = 4,  // Red
    J = 5,  // Blue
    L = 6   // Orange
};

class Tetromino {
private:
    TetrominoType type;
    int x, y;              // Current position (top-left)
    int rotation;          // 0, 1, 2, 3 for 4 rotation states
    
    // Shape definitions for each tetromino and rotation state
    // grid[rotation][row][col]
    static const bool shapes[7][4][4][4];
    
public:
    Tetromino(TetrominoType type = I, int startX = 3, int startY = 0);
    
    // Movement
    void moveLeft();
    void moveRight();
    void moveDown();
    void moveUp();
    
    // Rotation
    void rotate();
    void rotateClockwise();
    void rotateCounterClockwise();
    
    // Getters
    TetrominoType getType() const;
    int getX() const;
    int getY() const;
    int getRotation() const;
    const bool (*getShape() const)[4][4];
    
    // Setters
    void setPosition(int newX, int newY);
    
    // Get occupied cells
    std::vector<std::pair<int, int>> getOccupiedCells() const;
};

#endif
