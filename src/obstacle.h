#ifndef OBSTACLE_H
#define OBSTACLE_H

class Obstacle {
public:
  Obstacle(int x, int y) : _x(x), _y(y) {}

  // Getters
  int GetX() const { return _x; }
  int GetY() const { return _y; }

private:
  int _x;
  int _y;
};

#endif
