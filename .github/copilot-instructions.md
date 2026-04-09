# Project Guidelines

## Code Style
- Naming: Mixed camelCase for methods (e.g., `init()`, `render()`), snake_case for variables (e.g., `gridsize`, `post`). Headers use ALL_CAPS for constants (e.g., `N` in consts.h).
- Includes: Use relative paths for project headers (e.g., `#include "game/game.h"`).
- No namespaces: Everything in global scope.
- Charset: UTF-8 sources with GBK execution charset on Windows.

## Architecture
- Simple game loop: Initialize, interact, update effects, render in a loop.
- Core components: Game class manages state, Map for grid logic, render for drawing, interact for input.
- Raylib for rendering and input; no advanced patterns like ECS.

## Build and Test
- See [README.md](README.md) for build commands and dependencies.
- No automated tests; validate by running the game manually.
- Use build.bat on Windows, CMake or Makefile elsewhere.

## Conventions
- Turn-based mechanics: Add balls randomly, clear lines of 5+ for points (2^x score).
- Win at 1024 points, lose on full grid.
- Pathfinding ensures valid moves; highlight invalid paths.
- Effects use gradients and particles for smooth animations.