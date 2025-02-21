#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <chrono>

class Obstacle {
public:
  Obstacle(int x, int y) : _x(x), _y(y), creation_time(std::chrono::steady_clock::now())  {}

  // Getters
  int GetX() const { return _x; }
  int GetY() const { return _y; }

  std::chrono::steady_clock::time_point GetCreationTime() const {
    return creation_time;
  }

private:
  int _x;
  int _y;
  std::chrono::steady_clock::time_point creation_time;
};

#endif
