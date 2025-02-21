#include "game.h"
#include <iostream>
#include "SDL.h"
#include "controller.h"
#include <chrono>
#include <algorithm>

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  PlaceFood();
}

void Game::AddObstacle(){
  std::lock_guard<std::mutex> lock(obstacles_mutex);
  int x = random_w(engine);
  int y = random_h(engine);
  obstacles.emplace_back(Obstacle(x, y));
}

void Game::AddObstacle(int x, int y){
  std::lock_guard<std::mutex> lock(obstacles_mutex);
  obstacles.emplace_back(Obstacle(x, y));
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration, int loaded_high_score) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  obstacle_spawner_thread = std::thread(&Game::SpawnObstaclesLoop, this);

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake, *this);
    if (!IsPaused()){
      Update();
    }

    {
      std::lock_guard<std::mutex> lock(obstacles_mutex);
      renderer.Render(snake, food, obstacles);
    }

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      std::string old_title = paused ? "[PAUSED] " : "";
      renderer.UpdateWindowTitle(score, loaded_high_score, frame_count);

      if (paused) {
        // Retrieve the existing title
        std::string pausedTitle = "[PAUSED] Score: " +
                                  std::to_string(score) +
                                  " | High Score: " +
                                  std::to_string(loaded_high_score) +
                                  " | FPS: " + std::to_string(frame_count);
        SDL_SetWindowTitle(SDL_GetWindowFromID(1), pausedTitle.c_str());
      }
      
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
  game_running = false;
  if(obstacle_spawner_thread.joinable()){
    obstacle_spawner_thread.join();
  }
}

void Game::SpawnObstaclesLoop() {
  while (game_running) {
    // Sleep 5 seconds between spawns
    std::this_thread::sleep_for(std::chrono::seconds(5));

    if (!paused && game_running) {
      AddObstacle(); // create a new obstacle

      // remove obstacles older than 10 seconds
      auto now = std::chrono::steady_clock::now();
      {
        std::lock_guard<std::mutex> lock(obstacles_mutex);

        obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(),
          [&](Obstacle const &obs){
            auto age = now - obs.GetCreationTime();
            return age > std::chrono::seconds(10);
          }), 
          obstacles.end()
        );
      }
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by the snake or obstacles
    if (!snake.SnakeCell(x, y)) {
      // Also check obstacles
      bool collision_with_obstacle = false;

      // We'll lock the obstacle list to check
      {
        std::lock_guard<std::mutex> lock(obstacles_mutex);
        for (auto const &obs : obstacles) {
          if (obs.GetX() == x && obs.GetY() == y) {
            collision_with_obstacle = true;
            break;
          }
        }
      }

      if (!collision_with_obstacle) {
        food.x = x;
        food.y = y;
        return;
      }
    }
  }
}

void Game::Update() {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }

  {
    std::lock_guard<std::mutex> lock(obstacles_mutex);
    for (auto const &obs : obstacles) {
      if (obs.GetX() == new_x && obs.GetY() == new_y) {
        // if snake head collides with an obstacle -> snake is dead
        std::cout << "Snake collided with obstacle! Game Over!\n";
        snake.alive = false;
        break;
      }
    }
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }