# CPPND: Capstone Snake Game Example

This is a starter repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The code for this repo was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.

<img src="snake_game.gif"/>

## Dependencies for Running Locally

- **cmake >= 3.7**
  - All OSes: [click here for installation instructions](https://cmake.org/install/)
- **make >= 4.1** (Linux, Mac), 3.81 (Windows)
  - Linux: make is installed by default on most Linux distros
  - Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  - Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
- **SDL2 >= 2.0**
  - All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  - Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source.
- **gcc/g++ >= 5.4**
  - Linux: gcc / g++ is installed by default on most Linux distros
  - Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  - Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.

### Running from Windows using Docker Container

Running a GUI application inside a Docker Linux container on Windows requires an X server on your host system. Install an X Server on Windows. A popular choice is **VcXsrv**. Install and launch it with default settings.

1. Install Docker Desktop and run it.
2. `docker build -t capstone-cpp .`
3. `docker run -it -v "$(pwd -W):/workspace" capstone-cpp`
4. Inside the container run:

```
export DISPLAY=host.docker.internal:0
```

AFTER exporting the DISPLAY variable, run the game: `./SnakeGame`. make sure that VcXsrv is running.

## New Features & Behavior

1. **High Score System**

   - A new class `HighScoreManager` (`src/highscore_manager.h` / `.cpp`) reads/writes a file `highscore.txt`.
   - If the file doesn't exist, it is created with a default score of 0.
   - The previous high score is displayed in the window title, and if the new score at game-end exceeds it, `highscore.txt` is updated.

2. **Pause Feature**

   - Press **Space** to toggle pause/resume.
   - Implemented in `Controller::HandleInput` (see `src/controller.cpp`).
   - When paused, the snake is frozen, but the screen continues to update so that the user knows they're paused.

3. **Obstacles**

   - A new `Obstacle` class (`src/obstacle.h/.cpp`) holds `(x,y)` positions.
   - If the snake collides with an obstacle, it dies (Game Over).
   - Obstacles are drawn in a distinct color on-screen.

4. **Function Overloading for Obstacles**

   - `Game::AddObstacle()` is overloaded: one version randomly spawns an obstacle, another takes `(x, y)`.

5. **Multithreaded Obstacle Spawner**

   - A separate thread in `Game` spawns new obstacles every few seconds (see `SpawnObstaclesLoop()` in `game.cpp`).
   - A `std::mutex` protects `std::vector<Obstacle>` from race conditions.

6. **Timed Obstacle Removal**
   - Each obstacle has a creation timestamp.
   - After 10 seconds, older obstacles automatically disappear (in `SpawnObstaclesLoop()`).

## Where the Rubric Items are Addressed

### Loops, Functions, I/O

- **Reads data from a file**: `HighScoreManager::LoadHighScoreFromFile` & `SaveHighScoreToFile` (in `src/highscore_manager.cpp`).
- **Project accepts user input**: Space key toggles pause (in `Controller::HandleInput`, `src/controller.cpp`).

### Object Oriented Programming

- **New Classes**: `Obstacle` (`src/obstacle.h`) and `HighScoreManager` (`src/highscore_manager.h`).
- **Constructors with member init lists**: `Obstacle(int x,int y)`, `HighScoreManager()`.
- **Overloaded functions**: `AddObstacle()` in `Game` (see `src/game.h` ~ lines 25-26).

### Memory Management

- **References in function declarations**: e.g. `Controller::HandleInput(bool &running, Snake &snake, Game &game)`.
- **RAII**: `std::lock_guard<std::mutex>` in multiple places (e.g., in `Game::AddObstacle()`).
- **Use of standard containers**: `std::vector<Obstacle>` to store obstacles.

### Concurrency

- **Multiple threads**: `obstacle_spawner_thread` in `Game` (started in `Game::Run`).
- **Mutex**: `std::mutex obstacles_mutex` to protect shared state in `Game::Update()`, `AddObstacle()`, etc.

---

Enjoy the game, and feel free to reach out if you have any questions!
