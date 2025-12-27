#include "Board.h"
#include <algorithm>

Board::Board() {
    clear();
}

void Board::clear() {
    grid.assign(HEIGHT, std::vector<int>(WIDTH, -1));
}

bool Board::canPlace(const Tetromino& piece) const {
    auto cells = piece.getOccupiedCells();
    
    for (const auto& cell : cells) {
        int px = cell.first;
        int py = cell.second;
        
        // Check boundaries
        if (px < 0 || px >= WIDTH || py < 0 || py >= HEIGHT) {
            return false;
        }
        
        // Check collision with existing pieces
        if (grid[py][px] != -1) {
            return false;
        }
    }
    
    return true;
}

void Board::place(const Tetromino& piece) {
    auto cells = piece.getOccupiedCells();
    
    for (const auto& cell : cells) {
        int px = cell.first;
        int py = cell.second;
        
        if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT) {
            grid[py][px] = static_cast<int>(piece.getType());
        }
    }
}

int Board::clearLines() {
    int linesCleared = 0;
    int row = 0;
    
    while (row < HEIGHT) {
        bool isLineFull = true;
        
        for (int col = 0; col < WIDTH; col++) {
            if (grid[row][col] == -1) {
                isLineFull = false;
                break;
            }
        }
        
        if (isLineFull) {
            // Remove the full line
            grid.erase(grid.begin() + row);
            // Add empty line at top
            grid.insert(grid.begin(), std::vector<int>(WIDTH, -1));
            linesCleared++;
        } else {
            row++;
        }
    }
    
    return linesCleared;
}

bool Board::isGameOver() const {
    // Game is over if there are blocks in the top rows
    for (int col = 0; col < WIDTH; col++) {
        if (grid[0][col] != -1 || grid[1][col] != -1) {
            return true;
        }
    }
    return false;
}

int Board::getCell(int x, int y) const {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return -2; // Out of bounds indicator
    }
    return grid[y][x];
}
