#include "Tetromino.h"

// Define all tetromino shapes with 4 rotation states
// Each shape is a 4x4 grid
const bool Tetromino::shapes[7][4][4][4] = {
    // I-piece (light blue)
    {
        { {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0} },
        { {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0} }
    },
    // O-piece (yellow)
    {
        { {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} }
    },
    // T-piece (purple)
    {
        { {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0} },
        { {0, 0, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0} },
        { {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0} }
    },
    // S-piece (green)
    {
        { {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0} },
        { {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0} }
    },
    // Z-piece (red)
    {
        { {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0} },
        { {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0} }
    },
    // J-piece (blue)
    {
        { {0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0} },
        { {1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {1, 1, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0} },
        { {1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} }
    },
    // L-piece (orange)
    {
        { {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0} },
        { {1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0} },
        { {0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} }
    }
};

Tetromino::Tetromino(TetrominoType type, int startX, int startY)
    : type(type), x(startX), y(startY), rotation(0) {}

void Tetromino::moveLeft() {
    x--;
}

void Tetromino::moveRight() {
    x++;
}

void Tetromino::moveDown() {
    y++;
}

void Tetromino::moveUp() {
    y--;
}

void Tetromino::rotate() {
    rotation = (rotation + 1) % 4;
}

void Tetromino::rotateClockwise() {
    rotation = (rotation + 1) % 4;
}

void Tetromino::rotateCounterClockwise() {
    rotation = (rotation + 3) % 4;
}

TetrominoType Tetromino::getType() const {
    return type;
}

int Tetromino::getX() const {
    return x;
}

int Tetromino::getY() const {
    return y;
}

int Tetromino::getRotation() const {
    return rotation;
}

const bool (*Tetromino::getShape() const)[4][4] {
    return &shapes[type][rotation];
}

void Tetromino::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

std::vector<std::pair<int, int>> Tetromino::getOccupiedCells() const {
    std::vector<std::pair<int, int>> cells;
    const bool (*shape)[4][4] = getShape();
    
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if ((*shape)[row][col]) {
                cells.push_back({x + col, y + row});
            }
        }
    }
    return cells;
}
