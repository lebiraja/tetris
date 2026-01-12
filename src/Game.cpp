#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <SDL2/SDL.h>

Game::Game()
    : currentPiece(I), nextPiece(L), ghostPiece(I),
      holdPiece(nullptr), renderer(std::make_unique<Renderer>()),
      score(0), highScore(0), level(1), lines(0), frameCounter(0),
      gameOver(false), paused(false), running(true), canHold(true),
      state(GameState::TITLE), dropSpeed(60), animFrameCounter(0) {
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
                // Handle input based on current state
                if (state == GameState::TITLE) {
                    if (event.key.keysym.sym == SDLK_RETURN ||
                        event.key.keysym.sym == SDLK_SPACE) {
                        state = GameState::PLAYING;
                        resetGame();
                    } else if (event.key.keysym.sym == SDLK_q) {
                        running = false;
                    }
                } else if (state == GameState::PAUSED) {
                    if (event.key.keysym.sym == SDLK_p) {
                        state = GameState::PLAYING;
                        paused = false;
                    } else if (event.key.keysym.sym == SDLK_q) {
                        gameOver = true;
                        running = false;
                    }
                } else if (state == GameState::PLAYING) {
                    switch (event.key.keysym.sym) {
                        case SDLK_a:
                        case SDLK_LEFT:
                            movePieceLeft();
                            updateGhostPiece();
                            break;
                        case SDLK_d:
                        case SDLK_RIGHT:
                            movePieceRight();
                            updateGhostPiece();
                            break;
                        case SDLK_w:
                        case SDLK_UP:
                            rotatePiece();
                            updateGhostPiece();
                            break;
                        case SDLK_s:
                        case SDLK_DOWN:
                            movePieceDown();
                            break;
                        case SDLK_SPACE:
                            // Hard drop
                            while (movePieceDown());
                            break;
                        case SDLK_c:
                            holdCurrentPiece();
                            break;
                        case SDLK_p:
                            paused = true;
                            state = GameState::PAUSED;
                            break;
                        case SDLK_q:
                            gameOver = true;
                            running = false;
                            break;
                        default:
                            break;
                    }
                }
                break;
        }
    }
}

void Game::update() {
    if (state != GameState::PLAYING) return;
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
                int lineBonus[] = {0, 100, 300, 500, 800};  // Single, Double, Triple, Tetris
                score += lineBonus[clearedLines] * level;

                if (lines % 10 == 0) {
                    increaseLevel();
                }
            }

            spawnNewPiece();

            if (!board.canPlace(currentPiece)) {
                gameOver = true;
                state = GameState::GAME_OVER;
            }
        }
    }
}

void Game::render() {
    renderer->clear();

    switch (state) {
        case GameState::TITLE:
            renderer->renderTitleScreen();
            break;

        case GameState::PLAYING:
            renderer->renderGame(board, currentPiece, nextPiece, score, highScore, level, lines,
                                &ghostPiece, holdPiece.get(), canHold);
            break;

        case GameState::PAUSED:
            renderer->renderGame(board, currentPiece, nextPiece, score, highScore, level, lines,
                                &ghostPiece, holdPiece.get(), canHold);
            renderer->renderPauseScreen();
            break;

        case GameState::GAME_OVER:
            renderer->renderGameOver(score, highScore, level, lines);
            break;

        case GameState::LINE_CLEAR_ANIM:
            renderer->renderGame(board, currentPiece, nextPiece, score, highScore, level, lines,
                                &ghostPiece, holdPiece.get(), canHold);
            break;
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
    canHold = true;
    holdPiece.reset();

    // Spawn first piece
    spawnNewPiece();
}

void Game::run() {
    init();

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    while (running) {
        Uint32 frameStart = SDL_GetTicks();

        handleInput();
        if (!running) break;

        update();
        render();

        // Handle game over state
        if (state == GameState::GAME_OVER) {
            // Save high score if beaten
            if (score > highScore) {
                highScore = score;
                saveHighScore();
            }

            // Wait for retry or quit
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                } else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_r) {
                        resetGame();
                        state = GameState::PLAYING;
                    } else if (event.key.keysym.sym == SDLK_q) {
                        running = false;
                    }
                }
            }
        }

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

void Game::spawnNewPiece() {
    currentPiece = nextPiece;
    currentPiece.setPosition(3, 0);

    TetrominoType randomType = static_cast<TetrominoType>(rand() % 7);
    nextPiece = Tetromino(randomType);

    // Reset hold capability for new piece
    canHold = true;

    // Update ghost piece
    updateGhostPiece();
}

void Game::updateGhostPiece() {
    // Copy current piece to ghost
    ghostPiece = currentPiece;

    // Drop ghost piece to the bottom
    while (true) {
        ghostPiece.moveDown();
        if (!board.canPlace(ghostPiece)) {
            ghostPiece.moveUp();
            break;
        }
    }
}

void Game::holdCurrentPiece() {
    if (!canHold) return;

    if (holdPiece) {
        // Swap current with held piece
        Tetromino temp = currentPiece;
        currentPiece = *holdPiece;
        currentPiece.setPosition(3, 0);
        holdPiece = std::make_unique<Tetromino>(temp.getType());
    } else {
        // Store current piece and spawn new one
        holdPiece = std::make_unique<Tetromino>(currentPiece.getType());
        currentPiece = nextPiece;
        currentPiece.setPosition(3, 0);

        TetrominoType randomType = static_cast<TetrominoType>(rand() % 7);
        nextPiece = Tetromino(randomType);
    }

    canHold = false;  // Can only hold once per piece drop
    updateGhostPiece();
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
    dropSpeed = std::max(5, 60 - (level - 1) * 5);
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
