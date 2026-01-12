#include "Renderer.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

Renderer::Renderer(int screenWidth, int screenHeight)
    : window(nullptr), renderer(nullptr),
      blockSize(28), screenWidth(1000), screenHeight(750),
      backgroundColor({18, 18, 28, 255}),
      borderColor({80, 100, 140, 255}),
      gridColor({35, 40, 55, 255}),
      lineClearAnimFrame(0)
{
    // Modern, softer color palette for tetrominoes
    pieceColors[0] = {0, 210, 210, 255};      // I - Cyan
    pieceColors[1] = {240, 220, 60, 255};     // O - Yellow
    pieceColors[2] = {180, 80, 220, 255};     // T - Purple
    pieceColors[3] = {100, 220, 100, 255};    // S - Green
    pieceColors[4] = {240, 90, 90, 255};      // Z - Red
    pieceColors[5] = {90, 120, 240, 255};     // J - Blue
    pieceColors[6] = {240, 160, 80, 255};     // L - Orange

    // Calculate board position (centered more elegantly)
    boardX = 50;
    boardY = 50;
}

Renderer::~Renderer() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

void Renderer::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return;
    }

    window = SDL_CreateWindow(
        "TETRIS",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth,
        screenHeight,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return;
    }

    // Enable alpha blending
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(renderer,
                          backgroundColor.r,
                          backgroundColor.g,
                          backgroundColor.b,
                          backgroundColor.a);
}

void Renderer::clear() {
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, 255);
    SDL_RenderClear(renderer);
}

void Renderer::renderBlockAt(int x, int y, int size, SDL_Color color, bool isGhost) {
    if (isGhost) {
        // Ghost piece: just outline with transparency
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 80);
        SDL_Rect outline = {x, y, size, size};
        SDL_RenderDrawRect(renderer, &outline);
        SDL_Rect inner = {x + 1, y + 1, size - 2, size - 2};
        SDL_RenderDrawRect(renderer, &inner);
        return;
    }

    int margin = 2;

    // Main block fill
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_Rect rect = {x + margin, y + margin, size - margin * 2, size - margin * 2};
    SDL_RenderFillRect(renderer, &rect);

    // Top-left highlight (lighter)
    int highlightR = std::min(255, color.r + 60);
    int highlightG = std::min(255, color.g + 60);
    int highlightB = std::min(255, color.b + 60);
    SDL_SetRenderDrawColor(renderer, highlightR, highlightG, highlightB, 200);
    SDL_RenderDrawLine(renderer, x + margin, y + margin, x + size - margin - 1, y + margin);
    SDL_RenderDrawLine(renderer, x + margin, y + margin, x + margin, y + size - margin - 1);

    // Bottom-right shadow (darker)
    int shadowR = std::max(0, color.r - 50);
    int shadowG = std::max(0, color.g - 50);
    int shadowB = std::max(0, color.b - 50);
    SDL_SetRenderDrawColor(renderer, shadowR, shadowG, shadowB, 200);
    SDL_RenderDrawLine(renderer, x + size - margin - 1, y + margin + 1, x + size - margin - 1, y + size - margin - 1);
    SDL_RenderDrawLine(renderer, x + margin + 1, y + size - margin - 1, x + size - margin - 1, y + size - margin - 1);

    // Inner shine (small white square in top-left)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 60);
    SDL_Rect shine = {x + margin + 3, y + margin + 3, 4, 4};
    SDL_RenderFillRect(renderer, &shine);
}

void Renderer::renderBlock(int gridX, int gridY, SDL_Color color, bool isGhost) {
    int x = boardX + (gridX * blockSize);
    int y = boardY + (gridY * blockSize);
    renderBlockAt(x, y, blockSize, color, isGhost);
}

void Renderer::renderGame(const Board& board, const Tetromino& currentPiece,
                          const Tetromino& nextPiece, int score, int highScore, int level, int lines,
                          const Tetromino* ghostPiece,
                          const Tetromino* holdPiece,
                          bool canHold) {

    int boardWidth = 10 * blockSize;
    int boardHeight = 20 * blockSize;

    // ===== BOARD BACKGROUND =====
    // Outer glow effect
    SDL_SetRenderDrawColor(renderer, 40, 50, 80, 100);
    SDL_Rect outerGlow = {boardX - 8, boardY - 8, boardWidth + 16, boardHeight + 16};
    SDL_RenderFillRect(renderer, &outerGlow);

    // Main board background
    SDL_SetRenderDrawColor(renderer, 12, 14, 22, 255);
    SDL_Rect boardBg = {boardX - 4, boardY - 4, boardWidth + 8, boardHeight + 8};
    SDL_RenderFillRect(renderer, &boardBg);

    // Board border
    SDL_SetRenderDrawColor(renderer, 60, 80, 120, 255);
    SDL_RenderDrawRect(renderer, &boardBg);

    // Draw subtle grid
    SDL_SetRenderDrawColor(renderer, 30, 35, 50, 150);
    for (int i = 0; i <= 10; i++) {
        SDL_RenderDrawLine(renderer,
                          boardX + i * blockSize, boardY,
                          boardX + i * blockSize, boardY + boardHeight);
    }
    for (int i = 0; i <= 20; i++) {
        SDL_RenderDrawLine(renderer,
                          boardX, boardY + i * blockSize,
                          boardX + boardWidth, boardY + i * blockSize);
    }

    // ===== DRAW GHOST PIECE =====
    if (ghostPiece) {
        auto ghostCells = ghostPiece->getOccupiedCells();
        for (const auto& cell : ghostCells) {
            if (cell.second >= 0 && cell.second < board.getHeight()) {
                renderBlock(cell.first, cell.second,
                           pieceColors[ghostPiece->getType()], true);
            }
        }
    }

    // ===== DRAW LOCKED PIECES =====
    const auto& grid = board.getGrid();
    for (int row = 0; row < board.getHeight(); row++) {
        for (int col = 0; col < board.getWidth(); col++) {
            if (grid[row][col] != -1) {
                // Check if this row is being cleared (for animation)
                bool isClearing = false;
                for (int clearRow : linesToClear) {
                    if (row == clearRow) {
                        isClearing = true;
                        break;
                    }
                }

                if (isClearing && lineClearAnimFrame > 0) {
                    // Flash effect during line clear
                    SDL_Color flashColor = {255, 255, 255, 255};
                    if (lineClearAnimFrame % 4 < 2) {
                        renderBlock(col, row, flashColor);
                    } else {
                        renderBlock(col, row, pieceColors[grid[row][col]]);
                    }
                } else {
                    renderBlock(col, row, pieceColors[grid[row][col]]);
                }
            }
        }
    }

    // ===== DRAW CURRENT PIECE =====
    auto currentCells = currentPiece.getOccupiedCells();
    for (const auto& cell : currentCells) {
        if (cell.second >= 0 && cell.second < board.getHeight()) {
            renderBlock(cell.first, cell.second,
                       pieceColors[currentPiece.getType()]);
        }
    }

    // ===== RIGHT SIDE PANEL =====
    int panelX = boardX + boardWidth + 40;
    int panelY = boardY;

    // Title with glow effect
    SDL_SetRenderDrawColor(renderer, 60, 100, 180, 80);
    SDL_Rect titleGlow = {panelX - 15, panelY - 45, 220, 35};
    SDL_RenderFillRect(renderer, &titleGlow);
    renderText("TETRIS", panelX + 45, panelY - 38, {100, 180, 255, 255}, 3);

    // Decorative line under title
    SDL_SetRenderDrawColor(renderer, 60, 100, 160, 255);
    SDL_RenderDrawLine(renderer, panelX - 10, panelY, panelX + 200, panelY);

    // ===== HOLD PIECE BOX =====
    int holdBoxY = panelY + 15;
    SDL_SetRenderDrawColor(renderer, 25, 28, 40, 220);
    SDL_Rect holdBox = {panelX - 10, holdBoxY, 100, 90};
    SDL_RenderFillRect(renderer, &holdBox);
    SDL_SetRenderDrawColor(renderer, canHold ? (Uint8)60 : (Uint8)40, canHold ? (Uint8)90 : (Uint8)50, canHold ? (Uint8)140 : (Uint8)80, 255);
    SDL_RenderDrawRect(renderer, &holdBox);

    renderText("HOLD", panelX + 15, holdBoxY + 5, canHold ? SDL_Color{150, 180, 220, 255} : SDL_Color{80, 80, 100, 255});

    if (holdPiece) {
        auto holdCells = holdPiece->getOccupiedCells();
        SDL_Color holdColor = pieceColors[holdPiece->getType()];
        if (!canHold) {
            holdColor.r /= 2;
            holdColor.g /= 2;
            holdColor.b /= 2;
        }
        for (const auto& cell : holdCells) {
            int px = panelX + cell.first * 18 + 8;
            int py = holdBoxY + cell.second * 18 + 30;
            renderBlockAt(px, py, 16, holdColor);
        }
    }

    // ===== NEXT PIECE BOX =====
    int nextBoxY = holdBoxY + 100;
    SDL_SetRenderDrawColor(renderer, 25, 28, 40, 220);
    SDL_Rect nextBox = {panelX - 10, nextBoxY, 100, 90};
    SDL_RenderFillRect(renderer, &nextBox);
    SDL_SetRenderDrawColor(renderer, 60, 90, 140, 255);
    SDL_RenderDrawRect(renderer, &nextBox);

    renderText("NEXT", panelX + 15, nextBoxY + 5, {150, 180, 220, 255});

    auto nextCells = nextPiece.getOccupiedCells();
    for (const auto& cell : nextCells) {
        int px = panelX + cell.first * 18 + 8;
        int py = nextBoxY + cell.second * 18 + 30;
        renderBlockAt(px, py, 16, pieceColors[nextPiece.getType()]);
    }

    // ===== STATS BOX =====
    int statsBoxY = nextBoxY + 105;
    SDL_SetRenderDrawColor(renderer, 25, 28, 40, 220);
    SDL_Rect statsBox = {panelX - 10, statsBoxY, 200, 180};
    SDL_RenderFillRect(renderer, &statsBox);
    SDL_SetRenderDrawColor(renderer, 60, 90, 140, 255);
    SDL_RenderDrawRect(renderer, &statsBox);

    // Score
    renderText("SCORE", panelX, statsBoxY + 10, {120, 150, 200, 255});
    renderText(formatNumber(score), panelX + 10, statsBoxY + 30, {255, 240, 100, 255});

    // High Score
    renderText("BEST", panelX, statsBoxY + 55, {255, 200, 80, 255});
    renderText(formatNumber(highScore), panelX + 10, statsBoxY + 75, {255, 200, 80, 255});

    // Level
    renderText("LEVEL", panelX, statsBoxY + 100, {120, 150, 200, 255});
    renderText(std::to_string(level), panelX + 10, statsBoxY + 120, {100, 255, 180, 255});

    // Lines
    renderText("LINES", panelX, statsBoxY + 145, {120, 150, 200, 255});
    renderText(formatNumber(lines), panelX + 10, statsBoxY + 165, {255, 150, 200, 255});

    // ===== CONTROLS BOX (Bottom) =====
    int ctrlBoxY = boardY + boardHeight + 20;
    SDL_SetRenderDrawColor(renderer, 25, 28, 40, 200);
    SDL_Rect ctrlBox = {boardX - 4, ctrlBoxY, boardWidth + 8, 135};
    SDL_RenderFillRect(renderer, &ctrlBox);
    SDL_SetRenderDrawColor(renderer, 50, 70, 110, 255);
    SDL_RenderDrawRect(renderer, &ctrlBox);

    renderText("CONTROLS", boardX + 80, ctrlBoxY + 8, {100, 160, 220, 255});

    int ctrlTextY = ctrlBoxY + 30;
    SDL_Color ctrlColor = {160, 170, 190, 255};
    renderText("A/D   MOVE", boardX + 10, ctrlTextY, ctrlColor);
    renderText("W     ROTATE", boardX + 10, ctrlTextY + 18, ctrlColor);
    renderText("S     SOFT DROP", boardX + 10, ctrlTextY + 36, ctrlColor);
    renderText("SPACE HARD DROP", boardX + 10, ctrlTextY + 54, ctrlColor);
    renderText("C     HOLD", boardX + 10, ctrlTextY + 72, ctrlColor);
    renderText("P PAUSE  Q QUIT", boardX + 10, ctrlTextY + 90, {120, 140, 160, 255});
}

void Renderer::renderPauseScreen() {
    // Semi-transparent overlay
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_Rect overlay = {0, 0, screenWidth, screenHeight};
    SDL_RenderFillRect(renderer, &overlay);

    // Pause box
    int boxW = 300, boxH = 150;
    int boxX = (screenWidth - boxW) / 2;
    int boxY = (screenHeight - boxH) / 2;

    SDL_SetRenderDrawColor(renderer, 30, 35, 50, 240);
    SDL_Rect pauseBox = {boxX, boxY, boxW, boxH};
    SDL_RenderFillRect(renderer, &pauseBox);

    SDL_SetRenderDrawColor(renderer, 80, 120, 180, 255);
    SDL_RenderDrawRect(renderer, &pauseBox);

    // Outer glow
    SDL_SetRenderDrawColor(renderer, 60, 100, 160, 100);
    SDL_Rect glow = {boxX - 3, boxY - 3, boxW + 6, boxH + 6};
    SDL_RenderDrawRect(renderer, &glow);

    renderText("PAUSED", boxX + 90, boxY + 30, {100, 180, 255, 255}, 3);

    renderText("PRESS P TO RESUME", boxX + 55, boxY + 80, {180, 190, 210, 255});
    renderText("PRESS Q TO QUIT", boxX + 70, boxY + 110, {140, 140, 160, 255});
}

void Renderer::renderTitleScreen() {
    // Background with subtle gradient effect
    for (int y = 0; y < screenHeight; y += 4) {
        int shade = 18 + (y * 10 / screenHeight);
        SDL_SetRenderDrawColor(renderer, shade, shade, shade + 10, 255);
        SDL_Rect row = {0, y, screenWidth, 4};
        SDL_RenderFillRect(renderer, &row);
    }

    // Title with glow
    int titleX = screenWidth / 2 - 120;
    int titleY = 180;

    SDL_SetRenderDrawColor(renderer, 40, 80, 160, 80);
    SDL_Rect titleGlow = {titleX - 30, titleY - 20, 300, 80};
    SDL_RenderFillRect(renderer, &titleGlow);

    renderText("TETRIS", titleX, titleY, {80, 160, 255, 255}, 5);

    // Subtitle
    renderText("C++ EDITION", titleX + 55, titleY + 70, {100, 120, 160, 255}, 2);

    // Decorative tetrominoes
    int demoY = 320;
    for (int i = 0; i < 7; i++) {
        int px = 200 + i * 90;
        renderBlockAt(px, demoY, 24, pieceColors[i]);
        renderBlockAt(px + 26, demoY, 24, pieceColors[i]);
    }

    // Instructions
    int instY = 420;
    renderText("PRESS ENTER TO START", screenWidth / 2 - 130, instY, {180, 200, 230, 255}, 2);

    // Controls preview
    int ctrlY = 500;
    SDL_SetRenderDrawColor(renderer, 25, 28, 40, 200);
    SDL_Rect ctrlBox = {screenWidth / 2 - 200, ctrlY, 400, 140};
    SDL_RenderFillRect(renderer, &ctrlBox);
    SDL_SetRenderDrawColor(renderer, 50, 70, 110, 255);
    SDL_RenderDrawRect(renderer, &ctrlBox);

    renderText("CONTROLS", screenWidth / 2 - 50, ctrlY + 15, {100, 160, 220, 255});

    SDL_Color ctrlColor = {140, 150, 170, 255};
    renderText("A/D - MOVE   W - ROTATE", screenWidth / 2 - 140, ctrlY + 45, ctrlColor);
    renderText("S - SOFT DROP   SPACE - HARD DROP", screenWidth / 2 - 180, ctrlY + 70, ctrlColor);
    renderText("C - HOLD   P - PAUSE   Q - QUIT", screenWidth / 2 - 170, ctrlY + 95, ctrlColor);
}

void Renderer::renderGameOver(int score, int highScore, int level, int lines) {
    // Semi-transparent dark overlay
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_Rect overlay = {0, 0, screenWidth, screenHeight};
    SDL_RenderFillRect(renderer, &overlay);

    // Game over box
    int boxW = 500, boxH = 400;
    int boxX = (screenWidth - boxW) / 2;
    int boxY = (screenHeight - boxH) / 2 - 30;

    // Outer glow (red tint for game over)
    SDL_SetRenderDrawColor(renderer, 100, 30, 30, 100);
    SDL_Rect glow = {boxX - 6, boxY - 6, boxW + 12, boxH + 12};
    SDL_RenderFillRect(renderer, &glow);

    // Main box
    SDL_SetRenderDrawColor(renderer, 35, 25, 30, 250);
    SDL_Rect gameOverBox = {boxX, boxY, boxW, boxH};
    SDL_RenderFillRect(renderer, &gameOverBox);

    // Border
    SDL_SetRenderDrawColor(renderer, 200, 70, 70, 255);
    SDL_RenderDrawRect(renderer, &gameOverBox);

    // Game Over text
    renderText("GAME OVER", boxX + 130, boxY + 30, {255, 80, 80, 255}, 3);

    // Separator
    SDL_SetRenderDrawColor(renderer, 120, 60, 60, 255);
    SDL_RenderDrawLine(renderer, boxX + 50, boxY + 75, boxX + boxW - 50, boxY + 75);

    bool isNewHighScore = (score == highScore && score > 0);
    int textY = boxY + 95;

    if (isNewHighScore) {
        renderText("NEW HIGH SCORE!", boxX + 140, textY, {255, 220, 80, 255}, 2);
        textY += 40;
    }

    // Stats
    renderText("FINAL SCORE", boxX + 170, textY, {180, 200, 230, 255});
    renderText(formatNumber(score), boxX + 200, textY + 25, {255, 240, 100, 255}, 2);

    textY += 65;
    renderText("BEST SCORE", boxX + 175, textY, {255, 200, 80, 255});
    renderText(formatNumber(highScore), boxX + 200, textY + 25, {255, 200, 80, 255}, 2);

    textY += 65;
    renderText("LEVEL", boxX + 210, textY, {150, 200, 255, 255});
    renderText(std::to_string(level), boxX + 235, textY + 25, {100, 255, 180, 255}, 2);

    textY += 65;
    renderText("LINES CLEARED", boxX + 160, textY, {255, 150, 200, 255});
    renderText(formatNumber(lines), boxX + 220, textY + 25, {255, 150, 200, 255}, 2);

    // Actions
    int actionY = boxY + boxH - 55;
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderDrawLine(renderer, boxX + 50, actionY - 15, boxX + boxW - 50, actionY - 15);

    renderText("R - RETRY", boxX + 120, actionY, {100, 255, 120, 255});
    renderText("Q - QUIT", boxX + 290, actionY, {255, 100, 100, 255});
}

void Renderer::startLineClearAnimation(const std::vector<int>& lines) {
    linesToClear = lines;
    lineClearAnimFrame = 20;  // 20 frames of animation
}

bool Renderer::updateLineClearAnimation() {
    if (lineClearAnimFrame > 0) {
        lineClearAnimFrame--;
        return true;
    }
    linesToClear.clear();
    return false;
}

std::string Renderer::formatNumber(int number) {
    std::string numStr = std::to_string(number);
    std::string result;
    int count = 0;

    for (int i = numStr.length() - 1; i >= 0; i--) {
        if (count > 0 && count % 3 == 0) {
            result = "," + result;
        }
        result = numStr[i] + result;
        count++;
    }
    return result;
}

void Renderer::renderText(const std::string& text, int x, int y, SDL_Color color, int scale) {
    const int charWidth = 6;
    const int pixelSize = scale;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    int cursorX = x;

    for (char c : text) {
        std::vector<std::vector<int>> pattern;

        switch (toupper(c)) {
            case 'A':
                pattern = {{0,1,1,1,0}, {1,0,0,0,1}, {1,1,1,1,1}, {1,0,0,0,1}, {1,0,0,0,1}};
                break;
            case 'B':
                pattern = {{1,1,1,1,0}, {1,0,0,0,1}, {1,1,1,1,0}, {1,0,0,0,1}, {1,1,1,1,0}};
                break;
            case 'C':
                pattern = {{0,1,1,1,0}, {1,0,0,0,0}, {1,0,0,0,0}, {1,0,0,0,0}, {0,1,1,1,0}};
                break;
            case 'D':
                pattern = {{1,1,1,0,0}, {1,0,0,1,0}, {1,0,0,0,1}, {1,0,0,1,0}, {1,1,1,0,0}};
                break;
            case 'E':
                pattern = {{1,1,1,1,1}, {1,0,0,0,0}, {1,1,1,1,0}, {1,0,0,0,0}, {1,1,1,1,1}};
                break;
            case 'F':
                pattern = {{1,1,1,1,1}, {1,0,0,0,0}, {1,1,1,1,0}, {1,0,0,0,0}, {1,0,0,0,0}};
                break;
            case 'G':
                pattern = {{0,1,1,1,0}, {1,0,0,0,0}, {1,0,1,1,1}, {1,0,0,0,1}, {0,1,1,1,0}};
                break;
            case 'H':
                pattern = {{1,0,0,0,1}, {1,0,0,0,1}, {1,1,1,1,1}, {1,0,0,0,1}, {1,0,0,0,1}};
                break;
            case 'I':
                pattern = {{1,1,1,1,1}, {0,0,1,0,0}, {0,0,1,0,0}, {0,0,1,0,0}, {1,1,1,1,1}};
                break;
            case 'J':
                pattern = {{0,0,1,1,1}, {0,0,0,1,0}, {0,0,0,1,0}, {1,0,0,1,0}, {0,1,1,0,0}};
                break;
            case 'K':
                pattern = {{1,0,0,1,0}, {1,0,1,0,0}, {1,1,0,0,0}, {1,0,1,0,0}, {1,0,0,1,0}};
                break;
            case 'L':
                pattern = {{1,0,0,0,0}, {1,0,0,0,0}, {1,0,0,0,0}, {1,0,0,0,0}, {1,1,1,1,1}};
                break;
            case 'M':
                pattern = {{1,0,0,0,1}, {1,1,0,1,1}, {1,0,1,0,1}, {1,0,0,0,1}, {1,0,0,0,1}};
                break;
            case 'N':
                pattern = {{1,0,0,0,1}, {1,1,0,0,1}, {1,0,1,0,1}, {1,0,0,1,1}, {1,0,0,0,1}};
                break;
            case 'O':
                pattern = {{0,1,1,1,0}, {1,0,0,0,1}, {1,0,0,0,1}, {1,0,0,0,1}, {0,1,1,1,0}};
                break;
            case 'P':
                pattern = {{1,1,1,1,0}, {1,0,0,0,1}, {1,1,1,1,0}, {1,0,0,0,0}, {1,0,0,0,0}};
                break;
            case 'Q':
                pattern = {{0,1,1,1,0}, {1,0,0,0,1}, {1,0,0,0,1}, {1,0,0,1,0}, {0,1,1,0,1}};
                break;
            case 'R':
                pattern = {{1,1,1,1,0}, {1,0,0,0,1}, {1,1,1,1,0}, {1,0,1,0,0}, {1,0,0,1,0}};
                break;
            case 'S':
                pattern = {{0,1,1,1,1}, {1,0,0,0,0}, {0,1,1,1,0}, {0,0,0,0,1}, {1,1,1,1,0}};
                break;
            case 'T':
                pattern = {{1,1,1,1,1}, {0,0,1,0,0}, {0,0,1,0,0}, {0,0,1,0,0}, {0,0,1,0,0}};
                break;
            case 'U':
                pattern = {{1,0,0,0,1}, {1,0,0,0,1}, {1,0,0,0,1}, {1,0,0,0,1}, {0,1,1,1,0}};
                break;
            case 'V':
                pattern = {{1,0,0,0,1}, {1,0,0,0,1}, {1,0,0,0,1}, {0,1,0,1,0}, {0,0,1,0,0}};
                break;
            case 'W':
                pattern = {{1,0,0,0,1}, {1,0,0,0,1}, {1,0,1,0,1}, {1,1,0,1,1}, {1,0,0,0,1}};
                break;
            case 'X':
                pattern = {{1,0,0,0,1}, {0,1,0,1,0}, {0,0,1,0,0}, {0,1,0,1,0}, {1,0,0,0,1}};
                break;
            case 'Y':
                pattern = {{1,0,0,0,1}, {0,1,0,1,0}, {0,0,1,0,0}, {0,0,1,0,0}, {0,0,1,0,0}};
                break;
            case 'Z':
                pattern = {{1,1,1,1,1}, {0,0,0,1,0}, {0,0,1,0,0}, {0,1,0,0,0}, {1,1,1,1,1}};
                break;
            case '0':
                pattern = {{0,1,1,1,0}, {1,0,0,1,1}, {1,0,1,0,1}, {1,1,0,0,1}, {0,1,1,1,0}};
                break;
            case '1':
                pattern = {{0,0,1,0,0}, {0,1,1,0,0}, {0,0,1,0,0}, {0,0,1,0,0}, {0,1,1,1,0}};
                break;
            case '2':
                pattern = {{0,1,1,1,0}, {1,0,0,0,1}, {0,0,1,1,0}, {0,1,0,0,0}, {1,1,1,1,1}};
                break;
            case '3':
                pattern = {{1,1,1,1,0}, {0,0,0,0,1}, {0,1,1,1,0}, {0,0,0,0,1}, {1,1,1,1,0}};
                break;
            case '4':
                pattern = {{1,0,0,1,0}, {1,0,0,1,0}, {1,1,1,1,1}, {0,0,0,1,0}, {0,0,0,1,0}};
                break;
            case '5':
                pattern = {{1,1,1,1,1}, {1,0,0,0,0}, {1,1,1,1,0}, {0,0,0,0,1}, {1,1,1,1,0}};
                break;
            case '6':
                pattern = {{0,1,1,1,0}, {1,0,0,0,0}, {1,1,1,1,0}, {1,0,0,0,1}, {0,1,1,1,0}};
                break;
            case '7':
                pattern = {{1,1,1,1,1}, {0,0,0,1,0}, {0,0,1,0,0}, {0,1,0,0,0}, {1,0,0,0,0}};
                break;
            case '8':
                pattern = {{0,1,1,1,0}, {1,0,0,0,1}, {0,1,1,1,0}, {1,0,0,0,1}, {0,1,1,1,0}};
                break;
            case '9':
                pattern = {{0,1,1,1,0}, {1,0,0,0,1}, {0,1,1,1,1}, {0,0,0,0,1}, {0,1,1,1,0}};
                break;
            case ':':
                pattern = {{0,0,0,0,0}, {0,0,1,0,0}, {0,0,0,0,0}, {0,0,1,0,0}, {0,0,0,0,0}};
                break;
            case '!':
                pattern = {{0,0,1,0,0}, {0,0,1,0,0}, {0,0,1,0,0}, {0,0,0,0,0}, {0,0,1,0,0}};
                break;
            case '/':
                pattern = {{0,0,0,0,1}, {0,0,0,1,0}, {0,0,1,0,0}, {0,1,0,0,0}, {1,0,0,0,0}};
                break;
            case '-':
                pattern = {{0,0,0,0,0}, {0,0,0,0,0}, {1,1,1,1,1}, {0,0,0,0,0}, {0,0,0,0,0}};
                break;
            case ',':
                pattern = {{0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}, {0,0,1,0,0}, {0,1,0,0,0}};
                break;
            case '.':
                pattern = {{0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}, {0,0,1,0,0}};
                break;
            case ' ':
                pattern = {{0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}};
                break;
            default:
                pattern = {{0,1,1,1,0}, {1,0,0,0,1}, {1,0,0,0,1}, {1,0,0,0,1}, {0,1,1,1,0}};
                break;
        }

        // Draw the character pattern
        for (int row = 0; row < 5; row++) {
            for (int col = 0; col < 5; col++) {
                if (pattern[row][col]) {
                    SDL_Rect pixel = {cursorX + col * pixelSize, y + row * pixelSize, pixelSize, pixelSize};
                    SDL_RenderFillRect(renderer, &pixel);
                }
            }
        }

        cursorX += charWidth * pixelSize;
    }
}

void Renderer::present() {
    SDL_RenderPresent(renderer);
}

bool Renderer::isRunning() const {
    return window != nullptr;
}
