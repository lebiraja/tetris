#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <SDL2/SDL.h>

Game::Game() 
    : currentPiece(I), nextPiece(L), renderer(std::make_unique<Renderer>()),
      score(0), highScore(0), level(1), lines(0), frameCounter(0), 
      gameOver(false), paused(false), running(true), dropSpeed(60) {
    srand(static_cast<unsigned>(time(nullptr)));
    loadHighScore();
}

void Game::init() {
    renderer->init();
    board.clear();
    spawnNewPiece();
    std::cout << "Tetris Game Started! Window should open..." << std::endl;
}

void Game::handleInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                gameOver = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_a:
                        movePieceLeft();
                        break;
                    case SDLK_d:
                        movePieceRight();
                        break;
                    case SDLK_w:
                        rotatePiece();
                        break;
                    case SDLK_s:
                        movePieceDown();
                        break;
                    case SDLK_SPACE:
                        // Hard drop
                        while (movePieceDown());
                        break;
                    case SDLK_p:
                        paused = !paused;
                        break;
                    case SDLK_q:
                        gameOver = true;
                        running = false;
                        break;
                    default:
                        break;
                }
                break;
        }
    }
}

void Game::update() {
    if (gameOver || paused) return;
    
    frameCounter++;
    
    if (frameCounter >= dropSpeed) {
        frameCounter = 0;
        if (!movePieceDown()) {
            lockPiece();
            int clearedLines = board.clearLines();
            
            if (clearedLines > 0) {
                lines += clearedLines;
                // Score calculation: more lines at once = more points
                score += 100 * clearedLines * level;
                
                if (lines % 10 == 0) {
                    increaseLevel();
                }
            }
            
            spawnNewPiece();
            
            if (!board.canPlace(currentPiece)) {
                gameOver = true;
            }
        }
    }
}

void Game::render() {
    renderer->clear();
    
    if (gameOver && !paused) {
        renderer->renderGameOver(score, highScore, level, lines);
    } else {
        renderer->renderGame(board, currentPiece, nextPiece, score, highScore, level, lines);
    }
    
    renderer->present();
}

void Game::resetGame() {
    // Reset game state
    board.clear();
    score = 0;
    level = 1;
    lines = 0;
    frameCounter = 0;
    gameOver = false;
    paused = false;
    dropSpeed = 60;
    
    // Spawn first piece
    spawnNewPiece();
}

void Game::run() {
    init();
    
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    
    bool retrying = true;
    
    while (retrying && running) {
        // Main game loop
        while (running && !gameOver) {
            Uint32 frameStart = SDL_GetTicks();
            
            handleInput();
            if (!running) break;
            
            update();
            render();
            
            Uint32 frameTime = SDL_GetTicks() - frameStart;
            if (frameDelay > frameTime) {
                SDL_Delay(frameDelay - frameTime);
            }
        }
        
        // Save high score if beaten
        if (gameOver && running) {
            if (score > highScore) {
                highScore = score;
                saveHighScore();
            }
        }
        
        // Show game over screen and handle retry
        bool waitingForInput = true;
        while (running && waitingForInput) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        running = false;
                        waitingForInput = false;
                        retrying = false;
                        break;
                    case SDL_KEYDOWN:
                        if (event.key.keysym.sym == SDLK_r) {
                            // Retry the game
                            resetGame();
                            waitingForInput = false;
                        } else if (event.key.keysym.sym == SDLK_q) {
                            // Quit
                            running = false;
                            waitingForInput = false;
                            retrying = false;
                        }
                        break;
                }
            }
            
            if (waitingForInput) {
                renderer->clear();
                renderer->renderGameOver(score, highScore, level, lines);
                renderer->present();
                SDL_Delay(50);
            }
        }
    }
}

void Game::spawnNewPiece() {
    currentPiece = nextPiece;
    currentPiece.setPosition(3, 0);
    
    TetrominoType randomType = static_cast<TetrominoType>(rand() % 7);
    nextPiece = Tetromino(randomType);
}

bool Game::movePieceDown() {
    currentPiece.moveDown();
    
    if (board.canPlace(currentPiece)) {
        return true;
    }
    
    currentPiece.moveUp();
    return false;
}

bool Game::movePieceLeft() {
    currentPiece.moveLeft();
    
    if (board.canPlace(currentPiece)) {
        return true;
    }
    
    currentPiece.moveRight();
    return false;
}

bool Game::movePieceRight() {
    currentPiece.moveRight();
    
    if (board.canPlace(currentPiece)) {
        return true;
    }
    
    currentPiece.moveLeft();
    return false;
}

bool Game::rotatePiece() {
    currentPiece.rotate();
    
    if (board.canPlace(currentPiece)) {
        return true;
    }
    
    currentPiece.rotateCounterClockwise();
    return false;
}

void Game::lockPiece() {
    board.place(currentPiece);
}

void Game::increaseLevel() {
    level++;
    updateDropSpeed();
}

void Game::updateDropSpeed() {
    dropSpeed = std::max(10, 60 - (level - 1) * 5);
}

void Game::saveHighScore() {
    std::ofstream file("highscore.txt");
    if (file.is_open()) {
        file << highScore;
        file.close();
    }
}

void Game::loadHighScore() {
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    } else {
        highScore = 0;
    }
}
