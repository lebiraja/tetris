# Contributing to Tetris C++ Game

Thank you for your interest in contributing! This document provides guidelines and instructions for contributing to this project.

## 🎯 Ways to Contribute

### 1. Report Bugs
- Use GitHub Issues to report bugs
- Include detailed steps to reproduce
- Mention your OS and SDL2 version
- Attach screenshots if relevant

### 2. Suggest Features
- Open an issue with the "enhancement" label
- Describe the feature and its benefits
- Discuss implementation approach if possible

### 3. Submit Code
- Fix bugs
- Implement new features
- Improve performance
- Enhance documentation

## 🚀 Getting Started

### Fork and Clone
```bash
# Fork the repository on GitHub, then:
git clone https://github.com/YOUR_USERNAME/cgame.git
cd cgame
```

### Set Up Development Environment
```bash
# Install dependencies (Ubuntu/Debian)
sudo apt install build-essential cmake libsdl2-dev

# Build the project
mkdir -p build && cd build
cmake ..
make
```

### Create a Branch
```bash
git checkout -b feature/your-feature-name
# or
git checkout -b fix/bug-description
```

## 📝 Code Guidelines

### C++ Style
- Use **C++17** standard features
- Follow existing code formatting
- Use meaningful variable and function names
- Add comments for complex logic

### Example:
```cpp
// Good
int calculateScore(int linesCleared, int currentLevel) {
    return 100 * linesCleared * currentLevel;
}

// Avoid
int calc(int l, int lvl) {
    return 100*l*lvl;
}
```

### File Organization
- **Headers** go in `include/`
- **Implementation** goes in `src/`
- Keep related code together
- One class per file (generally)

### Memory Management
- Use smart pointers (`std::unique_ptr`, `std::shared_ptr`)
- Avoid raw `new`/`delete`
- Clean up SDL resources properly

## 🧪 Testing Your Changes

### Build and Test
```bash
cd build
make
./tetris
```

### Test Checklist
- [ ] Game starts without errors
- [ ] All controls work correctly
- [ ] No memory leaks (use `valgrind` if possible)
- [ ] UI displays properly
- [ ] Game logic works as expected
- [ ] No compiler warnings

### Run with Valgrind (Optional)
```bash
valgrind --leak-check=full ./tetris
```

## 📋 Pull Request Process

### Before Submitting
1. **Test thoroughly** - Make sure everything works
2. **Update documentation** - README, comments, etc.
3. **Follow code style** - Match existing patterns
4. **Commit logically** - One feature/fix per commit

### Commit Messages
Use clear, descriptive commit messages:

```bash
# Good
git commit -m "Add ghost piece preview feature"
git commit -m "Fix collision detection bug with I-piece"
git commit -m "Improve UI color contrast for better readability"

# Avoid
git commit -m "fixed stuff"
git commit -m "update"
```

### Submit Pull Request
1. Push your branch to your fork
2. Open a Pull Request on GitHub
3. Fill out the PR template
4. Link related issues
5. Wait for review

### PR Description Template
```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Performance improvement
- [ ] Documentation update

## Testing
How did you test this?

## Screenshots (if applicable)
Add screenshots for UI changes
```

## 🎨 Feature Ideas

### Beginner-Friendly
- Add sound effects using SDL_mixer
- Implement high score saving to file
- Create different color themes
- Add keyboard configuration

### Intermediate
- Ghost piece (preview where piece will land)
- Hold piece feature
- Particle effects for line clears
- Settings menu

### Advanced
- AI player implementation
- Multiplayer mode
- Super Rotation System (SRS)
- Replay system

## 🐛 Debugging Tips

### Common Issues

**SDL2 Initialization Fails:**
```cpp
if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
}
```

**Rendering Issues:**
- Check `SDL_RenderPresent()` is called
- Verify color values are in range [0, 255]
- Ensure window is created before rendering

**Collision Detection:**
- Print grid state for debugging
- Visualize occupied cells
- Test edge cases (boundaries, rotations)

## 📚 Resources

### SDL2 Documentation
- [SDL2 Wiki](https://wiki.libsdl.org/)
- [Lazy Foo's SDL Tutorials](http://lazyfoo.net/tutorials/SDL/)

### Tetris Guidelines
- [Tetris Wiki](https://tetris.wiki/)
- [Super Rotation System](https://tetris.wiki/Super_Rotation_System)

### C++ Resources
- [C++ Reference](https://en.cppreference.com/)
- [Modern C++ Guidelines](https://isocpp.github.io/CppCoreGuidelines/)

## 💬 Communication

### Questions?
- Open a GitHub Discussion
- Comment on related issues
- Ask in your Pull Request

### Code Review
- Be respectful and constructive
- Explain your reasoning
- Be open to feedback

## 🏆 Recognition

Contributors will be:
- Listed in the README
- Credited in release notes
- Part of the project's success!

---

**Thank you for contributing! Every improvement, no matter how small, makes this project better! 🎮✨**
