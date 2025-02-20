#ifndef HIGHSCORE_MANAGER_H
#define HIGHSCORE_MANAGER_H

#include <string>

/**
 * This class manages reading and writing the high score from/to a file.
 * It stores the high score internally, with getters and setters.
 */
class HighScoreManager {
public:
    // Constructor
    HighScoreManager();

    // Read high score from file
    void LoadHighScoreFromFile(const std::string &filename);

    // Write current high score to file
    void SaveHighScoreToFile(const std::string &filename) const;

    // Getter/Setter
    int GetHighScore() const;
    void SetHighScore(int score);

private:
    int high_score{0};  // Tracks the highest score read from file
};

#endif
