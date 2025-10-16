# Platformer Game

A 2D platformer game built with C++ and OpenGL featuring multiple levels, enemies, collectibles, and a timer-based challenge system.

## Overview

Navigate through progressively challenging levels filled with obstacles, enemies, and collectibles. Defeat enemies with projectiles, collect coins to increase your score, and reach the door to advance to the next level before time runs out.

## Screenshots

<p align="center">
  <img src="imgs/Screenshot%202025-10-16%20at%2010.44.51%20AM.png" width="45%" alt="Gameplay Screenshot 1">
  <img src="imgs/Screenshot%202025-10-16%20at%2010.45.12%20AM.png" width="45%" alt="Gameplay Screenshot 2">
</p>

<p align="center">
  <img src="imgs/Screenshot%202025-10-16%20at%2010.45.23%20AM.png" width="45%" alt="Gameplay Screenshot 3">
</p>

## Features

- **Multiple Levels**: 6 progressively difficult levels with unique layouts
- **Combat System**: Fire projectiles to defeat enemies (spikes) that shoot back
- **Power-ups**: Collect coins (score +10), stars, and mushrooms
- **Timer**: 59-second countdown per level
- **Physics Engine**: Gravity, jumping, and collision detection
- **FPS Counter**: Real-time performance monitoring
- **Player Stats**: Lives, health, score, and level tracking

## Requirements

### System Dependencies
- X11 development libraries
- OpenGL
- GLU
- GLX
- pthread support

### Ubuntu/Debian
```bash
sudo apt-get install libx11-dev libglu1-mesa-dev libgl1-mesa-dev
```

### Fedora/RHEL
```bash
sudo dnf install libX11-devel mesa-libGLU-devel mesa-libGL-devel
```

## Building

```bash
make
```

To clean build artifacts:
```bash
make clean
```

## Running

### Normal Start
```bash
./walk
```

### Start at Specific Level (cheat code)
```bash
./walk [1-5]
```

Example: `./walk 3` starts at level 3

## Controls

- **Arrow Left**: Move left
- **Arrow Right**: Move right
- **Arrow Up**: Jump
- **Space**: Fire projectile
- **ESC**: Start game / Pause / Resume / Main menu
- **Q**: Quit game

## Gameplay

### Objective
Reach the door in each level before the timer runs out while managing your health and lives.

### Game Elements
- **Walls (brown)**: Static obstacles
- **Coins (gold)**: +10 points
- **Spikes (red)**: Enemies that fire projectiles (-5 health per hit)
- **Mushrooms**: Obstacles
- **Stars**: Collectibles
- **Door**: Level exit
- **Enemy Projectiles**: -15 health per hit

### Player Stats
- **Lives**: Start with 3
- **Health**: 100 per life (game over when all lives lost)
- **Score**: Points from collecting coins
- **Timer**: 59 seconds per level (lose all lives when time expires)

### Difficulty Progression
Enemy projectiles spawn faster as levels increase, making later levels more challenging.

## Project Structure

- `walk.cpp`: Main game loop, rendering, and game logic
- `rkhosa.h/cpp`: Level system, player physics, sprites, and projectiles
- `cschmiedel.h/cpp`: Timer and FPS counter implementations
- `fonts.h`: Text rendering utilities
- `images/`: Game sprites and textures
- `libggfonts.a`: Font rendering library

## Team Members & Contributions

**Gordon Griesel** - Original framework and base game structure

**Rajveer Khosa** (rkhosa.h/cpp)
- Level design and tile map system (6 unique levels)
- Collision detection using bounding box intersection
- Player physics engine (gravity, jumping, friction)
- Sprite rendering system (mushrooms, doors, stars, spikes, coins)
- Projectile system (player and enemy bullets)
- Combat mechanics and damage system
- Image texture loading and OpenGL rendering
- Box2D collision detection library

**Christian Schmiedel** (cschmiedel.h/cpp)
- Timer system with pause/resume functionality
- FPS counter with real-time performance tracking
- Game logging system (INFO, WARNING, ERROR, DEBUG levels)
- Timer rendering and display
- Game state debugging utilities

## Presentation

[View the project presentation](https://docs.google.com/presentation/d/16_oMJwWh0BXH3GA5LaOvDv4Hje54xs9fBv5f-0SFk4Q/edit?usp=sharing)

## License

Educational project - CS 3350
