#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "renderer.h"
#include "snake.h"
#include "obstacle.h"
#include <thread>
#include <mutex>

class Controller;

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration, int loaded_high_score);
  int GetScore() const;
  int GetSize() const;

  void TogglePause(){paused = !paused;} // New Toggle pause state
  bool IsPaused() const {return paused;} // Check if paused

  void AddObstacle(int x, int y);
  void AddObstacle();

 private:
  Snake snake;
  SDL_Point food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  bool paused{false}; // New paused state
  std::vector<Obstacle> obstacles;
  std::mutex obstacles_mutex;
  std::thread obstacle_spawner_thread;
  bool game_running{true};
  void PlaceFood();
  void Update();
  void SpawnObstaclesLoop();
};

#endif