#pragma once
#include <string>

enum class Difficulty { EASY = 0, MEDIUM = 1, HARD = 2 };

class ScoreManager {
public:
    ScoreManager();
    void reset();
    void add(int points);
    int getTotal() const;
private:
    int totalScore;
};
