# Tetris Game - C++ with SDL2 Graphics 🎮

A fully functional, modern Tetris game implementation in C++ with beautiful SDL2 graphical rendering. Features a sleek dark theme UI, smooth 60 FPS gameplay, and classic Tetris mechanics!

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![C++](https://img.shields.io/badge/C++-17-blue.svg)
![SDL2](https://img.shields.io/badge/SDL2-2.x-green.svg)

---

## 🚀 Quick Start

### Prerequisites

You'll need the following installed on your Linux system:

- **C++ Compiler** (g++ 11.0 or higher)
- **CMake** (3.10 or higher)
- **Make**
- **SDL2** development libraries

### Installing Dependencies

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential cmake libsdl2-dev
```

#### Fedora/RHEL
```bash
sudo dnf install gcc-c++ cmake SDL2-devel
```

#### Arch Linux
```bash
sudo pacman -S base-devel cmake sdl2
```

### Build & Run

```bash
# Clone the repository
git clone <your-repo-url>
cd cgame

# Create build directory
mkdir -p build
cd build

# Build the project
cmake ..
make

# Run the game!
./tetris
```

---

## 🎮 Game Controls

| Key | Action |
|-----|--------|
| **A** / **D** | Move piece left/right |
| **W** | Rotate piece clockwise |
| **S** | Soft drop (faster fall) |
| **SPACE** | Hard drop (instant fall) |
| **P** | Pause/Resume game |
| **Q** | Quit game |

---

## ✨ Features

### Core Gameplay
- ✅ **7 Classic Tetromino Pieces** - I, O, T, S, Z, J, L shapes
- ✅ **4-State Rotation System** - Smooth piece rotation with wall-kick detection
- ✅ **Collision Detection** - Precise boundary and piece stacking detection
- ✅ **Line Clearing** - Complete lines disappear with gravity effect
- ✅ **Progressive Difficulty** - Speed increases every 10 lines cleared
- ✅ **Score System** - Points based on lines cleared × level multiplier
- ✅ **Next Piece Preview** - See what's coming next
- ✅ **Game Over Detection** - Automatic detection when pieces reach top

### Graphics & UI
- 🎨 **Modern Dark Theme** - Professional blue-accented color scheme
- 🎨 **Smooth 60 FPS Rendering** - Buttery smooth gameplay
- 🎨 **Color-Coded Pieces** - Each tetromino has a distinct vibrant color
- 🎨 **Real-time Stats Display** - Score, level, and lines cleared
- 🎨 **Grid Overlay** - Visual guide for precise placement
- 🎨 **Game Over Screen** - Beautiful overlay with final statistics

---

## 📁 Project Structure

```
cgame/
├── CMakeLists.txt          # CMake build configuration
├── .gitignore              # Git ignore rules
├── README.md               # This file
│
├── include/                # Header files
│   ├── Game.h             # Main game loop and state management
│   ├── Board.h            # 10×20 game board logic
│   ├── Tetromino.h        # Tetromino pieces and rotation
│   ├── Player.h           # Player input handling
│   └── Renderer.h         # SDL2 graphics rendering
│
├── src/                    # Implementation files
│   ├── main.cpp           # Entry point
│   ├── Game.cpp           # Game logic implementation
│   ├── Board.cpp          # Board management & collision
│   ├── Tetromino.cpp      # Piece definitions & movement
│   ├── Player.cpp         # Player controls
│   └── Renderer.cpp       # SDL2 rendering engine
│
└── build/                  # Build output (generated)
    └── tetris             # Compiled executable
```

---

## 🏗️ Architecture Overview

### Class Hierarchy

#### `Renderer` Class
- **SDL2 Integration** - Window creation and management
- **Color Management** - Modern dark theme with accent colors
- **Block Rendering** - Draws tetromino blocks with borders
- **UI Components** - Score panel, next piece preview, controls guide
- **Custom Text Rendering** - Pixel-perfect bitmap font system

#### `Tetromino` Class
- **7 Piece Types** - Complete rotation matrices (4 states each)
- **Movement Methods** - `moveLeft()`, `moveRight()`, `moveDown()`
- **Rotation Logic** - `rotate()`, `rotateClockwise()`, `rotateCounterClockwise()`
- **Position Tracking** - `getOccupiedCells()` returns all block positions

#### `Board` Class
- **10×20 Grid** - Standard Tetris board dimensions
- **Collision Detection** - `canPlace()` validates piece placement
- **Piece Locking** - `place()` locks pieces permanently
- **Line Clearing** - `clearLines()` removes full rows and applies gravity
- **Game State** - `isGameOver()` checks win/loss conditions

#### `Game` Class
- **Game Loop** - Classic Input → Update → Render cycle
- **SDL2 Event Handling** - Keyboard and window events
- **Frame Timing** - 60 FPS with configurable drop speed
- **Score Calculation** - Bonus points for clearing multiple lines
- **State Management** - Running, paused, and game over states

---

## 🎯 Game Rules

1. **Piece Spawning** - New tetromino appears at top center of board
2. **Gravity** - Pieces automatically fall at increasing speeds
3. **Movement** - Player controls horizontal movement and rotation
4. **Collision** - Pieces cannot overlap or go out of bounds
5. **Locking** - Piece locks in place when it can't move down
6. **Line Clearing** - Full rows (10 blocks) are removed
7. **Scoring** - Formula: `100 × lines_cleared × current_level`
8. **Level Up** - Every 10 lines cleared increases level and speed
9. **Game Over** - Game ends when new piece can't be placed
10. **Hard Drop** - Space bar instantly drops piece to bottom

---

## 🛠️ Technical Details

### Technologies Used
- **Language**: C++17
- **Graphics**: SDL2 (Simple DirectMedia Layer)
- **Build System**: CMake 3.10+
- **Platform**: Linux (primary), cross-platform compatible

### Key Features
- **Window Size**: 1000×750 pixels
- **Frame Rate**: 60 FPS with VSync
- **Rendering**: Double-buffered SDL2 rendering
- **Input**: Event-driven (non-blocking)
- **Memory**: Smart pointers for automatic cleanup
- **Dependencies**: SDL2 only (no other external libraries)

### Color Palette
- **Background**: Dark blue theme `{15, 15, 25}`
- **Accent**: Modern blue `{80, 120, 180}`
- **Pieces**: 7 vibrant colors (cyan, yellow, purple, green, red, blue, orange)

---

## 🤝 Contributing

**I'm looking for contributors to help improve this game!** Here are some ideas:

### 🌟 Enhancement Ideas

#### High Priority
- [ ] **High Score Persistence** - Save/load high scores to file
- [ ] **Sound Effects** - Add audio for piece placement, line clears, game over
- [ ] **Ghost Piece** - Show transparent preview of where piece will land
- [ ] **Hold Piece Feature** - Store and swap current piece
- [ ] **Particle Effects** - Visual effects for line clears

#### Medium Priority
- [ ] **Different Game Modes** - Marathon, Sprint (40 lines), Zen mode
- [ ] **Improved Rotation** - SRS (Super Rotation System) with T-spins
- [ ] **Better Graphics** - Textures, gradients, animations
- [ ] **Settings Menu** - Customize controls, difficulty, theme
- [ ] **Statistics Tracking** - Track games played, best score, etc.

#### Advanced Features
- [ ] **AI Player** - Bot that plays automatically
- [ ] **Multiplayer Mode** - Local or network-based versus mode
- [ ] **Attack Mechanics** - Send garbage lines to opponent
- [ ] **Custom Themes** - User-selectable color schemes
- [ ] **Replay System** - Record and playback games
- [ ] **Leaderboards** - Online high score tracking

### How to Contribute

1. **Fork the repository**
2. **Create a feature branch** (`git checkout -b feature/amazing-feature`)
3. **Make your changes** and test thoroughly
4. **Commit your changes** (`git commit -m 'Add amazing feature'`)
5. **Push to the branch** (`git push origin feature/amazing-feature`)
6. **Open a Pull Request**

### Contribution Guidelines

- Follow existing code style and conventions
- Add comments for complex logic
- Test your changes before submitting
- Update README if adding new features
- Keep commits focused and atomic

---

## 📚 Learning Resources

This project is perfect for learning:

### Object-Oriented Programming
- Class design and encapsulation
- Separation of concerns
- Interface design

### Game Development
- Game loop architecture
- Collision detection algorithms
- State management
- Frame-based timing
- Input handling

### Graphics Programming
- SDL2 window management
- 2D rendering techniques
- Color management
- Frame pacing and VSync

### C++ Concepts
- Smart pointers (`std::unique_ptr`)
- STL containers (vectors, pairs)
- Multi-dimensional arrays
- Dynamic library linking

---

## 🐛 Troubleshooting

### Build Errors

**SDL2 not found:**
```bash
# Make sure SDL2 development headers are installed
sudo apt install libsdl2-dev
```

**CMake version too old:**
```bash
# Update CMake or modify CMakeLists.txt minimum version
cmake_minimum_required(VERSION 3.10)
```

### Runtime Issues

**Window doesn't appear:**
- Check if SDL2 is properly installed
- Verify graphics drivers are up to date
- Try running from terminal to see error messages

**Game runs too fast/slow:**
- VSync should limit to 60 FPS
- Check system performance
- Modify `dropSpeed` in Game.cpp if needed

---

## 📝 License

This project is open source and available for learning and improvement. Feel free to use, modify, and distribute.

---

## 🙏 Acknowledgments

- Classic Tetris gameplay mechanics
- SDL2 library for graphics
- Modern C++ best practices

---

## 📞 Contact & Support

- **Issues**: Please open an issue on GitHub
- **Pull Requests**: Always welcome!
- **Questions**: Feel free to ask in discussions

---

**Happy coding! Let's make this the best open-source Tetris implementation! 🎮✨**
