#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "highscore_manager.h"

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight);
  HighScoreManager highscore_manager;
  highscore_manager.LoadHighScoreFromFile("highscore.txt");
  std::cout << "Previous High Score: " 
            << highscore_manager.GetHighScore() << "\n";
  game.Run(controller, renderer, kMsPerFrame, highscore_manager.GetHighScore());
  int finalScore = game.GetScore();
  if (finalScore > highscore_manager.GetHighScore()) {
    highscore_manager.SetHighScore(finalScore);
  }
  highscore_manager.SaveHighScoreToFile("highscore.txt");
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << finalScore << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  return 0;
}
