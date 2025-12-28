#include "Renderer.h"
#include <iostream>
#include <sstream>

Renderer::Renderer(int screenWidth, int screenHeight)
    : window(nullptr), renderer(nullptr), 
      blockSize(25), screenWidth(1000), screenHeight(750),
      backgroundColor({15, 15, 25, 255}),
      borderColor({100, 150, 200, 255}),
      gridColor({40, 45, 60, 255})
{
    // Define colors for each tetromino type
    pieceColors[0] = {0, 240, 240, 255};      // I - Cyan
    pieceColors[1] = {240, 240, 0, 255};      // O - Yellow
    pieceColors[2] = {160, 0, 240, 255};      // T - Purple
    pieceColors[3] = {0, 240, 0, 255};        // S - Green
    pieceColors[4] = {240, 0, 0, 255};        // Z - Red
    pieceColors[5] = {0, 0, 240, 255};        // J - Blue
    pieceColors[6] = {240, 160, 0, 255};      // L - Orange
    
    // Calculate board position (left side with padding)
    boardX = 30;
    boardY = 30;
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
        "Tetris - C++ Edition",
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
    
    SDL_SetRenderDrawColor(renderer, 
                          backgroundColor.r, 
                          backgroundColor.g, 
                          backgroundColor.b, 
                          backgroundColor.a);
}

void Renderer::clear() {
    SDL_RenderClear(renderer);
}

void Renderer::renderBlock(int gridX, int gridY, SDL_Color color) {
    int x = boardX + (gridX * blockSize);
    int y = boardY + (gridY * blockSize);
    
    // Draw solid block
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {x + 1, y + 1, blockSize - 2, blockSize - 2};
    SDL_RenderFillRect(renderer, &rect);
    
    // Draw border
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
    SDL_RenderDrawRect(renderer, &rect);
}

void Renderer::renderGame(const Board& board, const Tetromino& currentPiece,
                          const Tetromino& nextPiece, int score, int highScore, int level, int lines) {
    // Draw board background with gradient effect
    SDL_SetRenderDrawColor(renderer, 25, 28, 40, 255);
    SDL_Rect boardBg = {boardX - 5, boardY - 5, 
                        10 * blockSize + 10, 20 * blockSize + 10};
    SDL_RenderFillRect(renderer, &boardBg);
    
    // Draw board border with accent color
    SDL_SetRenderDrawColor(renderer, 80, 120, 180, 255);
    SDL_RenderDrawRect(renderer, &boardBg);
    
    // Draw grid lines
    SDL_SetRenderDrawColor(renderer, 50, 50, 70, 100);
    for (int i = 0; i <= 10; i++) {
        SDL_RenderDrawLine(renderer, 
                          boardX + i * blockSize, boardY,
                          boardX + i * blockSize, boardY + 20 * blockSize);
    }
    for (int i = 0; i <= 20; i++) {
        SDL_RenderDrawLine(renderer, 
                          boardX, boardY + i * blockSize,
                          boardX + 10 * blockSize, boardY + i * blockSize);
    }
    
    // Draw locked pieces
    const auto& grid = board.getGrid();
    for (int row = 0; row < board.getHeight(); row++) {
        for (int col = 0; col < board.getWidth(); col++) {
            if (grid[row][col] != -1) {
                renderBlock(col, row, pieceColors[grid[row][col]]);
            }
        }
    }
    
    // Draw current falling piece
    auto currentCells = currentPiece.getOccupiedCells();
    for (const auto& cell : currentCells) {
        if (cell.second >= 0 && cell.second < board.getHeight()) {
            renderBlock(cell.first, cell.second, 
                       pieceColors[currentPiece.getType()]);
        }
    }
    
    // Draw UI Panel (right side)
    int panelX = boardX + 10 * blockSize + 40;
    int panelY = boardY;
    
    // Title with better color and styling
    renderText("T E T R I S", panelX - 15, panelY - 30, {100, 200, 255, 255});
    
    // Draw separator line with accent color
    SDL_SetRenderDrawColor(renderer, 80, 120, 180, 255);
    SDL_RenderDrawLine(renderer, panelX - 10, panelY + 5, panelX + 200, panelY + 5);
    
    // Stats Panel Background
    SDL_SetRenderDrawColor(renderer, 20, 25, 35, 180);
    SDL_Rect statsBox = {panelX - 10, panelY + 15, 210, 145};
    SDL_RenderFillRect(renderer, &statsBox);
    SDL_SetRenderDrawColor(renderer, 60, 100, 150, 255);
    SDL_RenderDrawRect(renderer, &statsBox);
    
    // Score section with improved colors
    renderText("SCORE", panelX, panelY + 25, {150, 180, 220, 255});
    renderText(std::to_string(score), panelX + 10, panelY + 42, {255, 255, 100, 255});
    
    // High Score section with trophy color
    renderText("HIGH SCORE", panelX, panelY + 65, {255, 215, 0, 255});
    renderText(std::to_string(highScore), panelX + 10, panelY + 82, {255, 215, 0, 255});
    
    // Level section
    renderText("LEVEL", panelX, panelY + 105, {150, 180, 220, 255});
    renderText(std::to_string(level), panelX + 10, panelY + 122, {100, 255, 150, 255});
    
    // Lines section
    renderText("LINES", panelX, panelY + 145, {150, 180, 220, 255});
    renderText(std::to_string(lines), panelX + 10, panelY + 162, {255, 150, 200, 255});
    
    // Next piece label
    renderText("NEXT PIECE", panelX, panelY + 195, {150, 180, 220, 255});
    SDL_SetRenderDrawColor(renderer, 20, 25, 35, 180);
    SDL_Rect nextBox = {panelX - 10, panelY + 210, 150, 110};
    SDL_RenderFillRect(renderer, &nextBox);
    SDL_SetRenderDrawColor(renderer, 80, 120, 180, 255);
    SDL_RenderDrawRect(renderer, &nextBox);
    
    // Draw next piece preview with better positioning
    auto nextCells = nextPiece.getOccupiedCells();
    for (const auto& cell : nextCells) {
        int px = panelX + cell.first * 22 + 22;
        int py = panelY + cell.second * 22 + 235;
        SDL_Rect block = {px, py, 18, 18};
        SDL_SetRenderDrawColor(renderer, pieceColors[nextPiece.getType()].r,
                              pieceColors[nextPiece.getType()].g,
                              pieceColors[nextPiece.getType()].b, 255);
        SDL_RenderFillRect(renderer, &block);
        // Add border to preview blocks
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
        SDL_RenderDrawRect(renderer, &block);
    }
    
    // Controls Guide (Bottom left) - Fixed layout
    int ctrlY = boardY + 20 * blockSize + 20;
    SDL_SetRenderDrawColor(renderer, 25, 28, 40, 255);
    SDL_Rect ctrlBox = {boardX - 5, ctrlY, 10 * blockSize + 10, 160};
    SDL_RenderFillRect(renderer, &ctrlBox);
    SDL_SetRenderDrawColor(renderer, 80, 120, 180, 255);
    SDL_RenderDrawRect(renderer, &ctrlBox);
    
    renderText("CONTROLS:", boardX + 5, ctrlY + 8, {100, 200, 255, 255});
    
    renderText("A/D  - MOVE LEFT/RIGHT", boardX + 8, ctrlY + 32, {200, 210, 220, 255});
    renderText("W    - ROTATE PIECE", boardX + 8, ctrlY + 52, {200, 210, 220, 255});
    renderText("SPACE - HARD DROP", boardX + 8, ctrlY + 72, {200, 210, 220, 255});
    renderText("S    - SOFT DROP", boardX + 8, ctrlY + 92, {200, 210, 220, 255});
    renderText("P    - PAUSE", boardX + 8, ctrlY + 112, {200, 210, 220, 255});
    renderText("Q    - QUIT", boardX + 8, ctrlY + 132, {200, 210, 220, 255});
}

void Renderer::renderGameOver(int score, int highScore, int level, int lines) {
    // Semi-transparent overlay
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_Rect overlay = {0, 0, screenWidth, screenHeight};
    SDL_RenderFillRect(renderer, &overlay);
    
    // Game Over box background with gradient effect
    SDL_SetRenderDrawColor(renderer, 40, 20, 30, 240);
    SDL_Rect gameOverBox = {150, 120, screenWidth - 300, 480};
    SDL_RenderFillRect(renderer, &gameOverBox);
    
    // Game Over box border with glow effect
    SDL_SetRenderDrawColor(renderer, 255, 80, 80, 255);
    SDL_RenderDrawRect(renderer, &gameOverBox);
    SDL_SetRenderDrawColor(renderer, 200, 50, 50, 150);
    SDL_Rect outerBorder = {gameOverBox.x - 2, gameOverBox.y - 2, gameOverBox.w + 4, gameOverBox.h + 4};
    SDL_RenderDrawRect(renderer, &outerBorder);
    
    // Game Over text with larger size
    renderText("G A M E  O V E R !", 260, 150, {255, 80, 80, 255});
    
    // Separator line
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderDrawLine(renderer, 220, 190, 580, 190);
    
    // Stats with improved layout
    bool newHighScore = (score == highScore && score > 0);
    
    if (newHighScore) {
        renderText("NEW HIGH SCORE!", 280, 215, {255, 215, 0, 255});
    }
    
    renderText("FINAL SCORE", 280, newHighScore ? 255 : 220, {200, 220, 255, 255});
    renderText(std::to_string(score), 300, newHighScore ? 275 : 240, {255, 255, 100, 255});
    
    renderText("HIGH SCORE", 280, newHighScore ? 315 : 290, {255, 215, 0, 255});
    renderText(std::to_string(highScore), 300, newHighScore ? 335 : 310, {255, 215, 0, 255});
    
    renderText("LEVEL REACHED", 280, newHighScore ? 375 : 360, {150, 220, 255, 255});
    renderText(std::to_string(level), 300, newHighScore ? 395 : 380, {100, 255, 200, 255});
    
    renderText("LINES CLEARED", 280, newHighScore ? 435 : 430, {255, 150, 200, 255});
    renderText(std::to_string(lines), 300, newHighScore ? 455 : 450, {255, 150, 200, 255});
    
    // Footer
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderDrawLine(renderer, 220, newHighScore ? 495 : 485, 580, newHighScore ? 495 : 485);
    renderText("THANKS FOR PLAYING!", 270, newHighScore ? 510 : 500, {200, 200, 200, 255});
    renderText("CLOSE WINDOW TO EXIT", 260, newHighScore ? 540 : 530, {150, 150, 150, 255});
    
    // Retry instructions with highlighted colors
    SDL_SetRenderDrawColor(renderer, 100, 255, 100, 255);
    SDL_RenderDrawLine(renderer, 220, newHighScore ? 565 : 555, 580, newHighScore ? 565 : 555);
    renderText("PRESS R TO RETRY", 280, newHighScore ? 575 : 565, {100, 255, 100, 255});
    renderText("PRESS Q TO QUIT", 290, newHighScore ? 605 : 595, {255, 100, 100, 255});
}

void Renderer::renderText(const std::string& text, int x, int y, SDL_Color color) {
    // Simple pixel font rendering using rectangles
    // Each character is drawn as a 5x7 pixel grid
    
    const int charWidth = 6;
    const int charHeight = 8;
    const int pixelSize = 2;
    
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    int cursorX = x;
    
    for (char c : text) {
        // Simple uppercase letters and common characters
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
            case '|':
                pattern = {{0,0,1,0,0}, {0,0,1,0,0}, {0,0,1,0,0}, {0,0,1,0,0}, {0,0,1,0,0}};
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
