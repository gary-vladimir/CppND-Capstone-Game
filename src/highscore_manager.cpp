#include "highscore_manager.h"
#include <fstream>
#include <iostream>

HighScoreManager::HighScoreManager() : high_score(0) {}

void HighScoreManager::LoadHighScoreFromFile(const std::string &filename) {
    std::ifstream file_in(filename);
    if (!file_in.is_open()) {
        // If file doesn't exist, create it with default 0
        std::ofstream file_out(filename);
        file_out << 0;
        file_out.close();
        high_score = 0;
        // No error message needed, since we're gracefully handling it
    } else {
        // Otherwise, read the file normally
        file_in >> high_score;
        file_in.close();
    }
}

void HighScoreManager::SaveHighScoreToFile(const std::string &filename) const {
    std::ofstream file_out(filename);
    if (file_out.is_open()) {
        file_out << high_score;
        file_out.close();
    } else {
        std::cerr << "Could not open high score file for writing.\n";
    }
}

int HighScoreManager::GetHighScore() const {
    return high_score;
}

void HighScoreManager::SetHighScore(int score) {
    high_score = score;
}
