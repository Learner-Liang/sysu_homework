# Airplane Game

A classic airplane shooting game built with Qt.

## Features

- **Player Control**: Move your green aircraft left/right using arrow keys or mouse
- **Three Enemy Types**:
  - Red enemies (slow, 10 points)
  - Yellow enemies (medium, 20 points)
  - Purple enemies (fast, 30 points)
- **Shooting System**: Press spacebar or click to shoot bullets
- **Collision Detection**: 
  - Player-enemy collisions reduce health by 10
  - Bullet-enemy collisions destroy enemies and add points
- **Health System**: Start with 100 health, game over at 0
- **Level System**: Difficulty increases every 100 points
- **Score Tracking**: Track your progress and try to beat your high score

## Requirements

- Qt5 or Qt6 (Widgets module)
- CMake 3.16 or higher
- C++17 compatible compiler

## Building

```bash
cd airplane-game
mkdir build
cd build
cmake ..
cmake --build .
```

## Running

```bash
./airplane-game
```

## Controls

- **Arrow Left**: Move player left
- **Arrow Right**: Move player right
- **Mouse Move**: Move player to follow mouse position
- **Space Bar**: Shoot bullet
- **Left Mouse Click**: Shoot bullet

## Game Rules

1. Start with 100 health
2. Each collision with an enemy reduces health by 10
3. Destroy enemies by shooting them with bullets
4. Different colored enemies give different points:
   - Red: 10 points
   - Yellow: 20 points
   - Purple: 30 points
5. Level increases every 100 points
6. Enemy spawn rate increases with each level
7. Game ends when health reaches 0

## Project Structure

- `main.cpp` - Application entry point
- `gamewindow.h/cpp` - Main game window with rendering and input handling
- `game.h/cpp` - Core game logic and state management
- `player.h/cpp` - Player aircraft implementation
- `enemy.h/cpp` - Enemy aircraft implementation with multiple types
- `bullet.h/cpp` - Bullet projectile implementation
- `CMakeLists.txt` - Build configuration

## License

This is a student project for educational purposes.
