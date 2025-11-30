#include "ScoreManager.h"

ScoreManager::ScoreManager() { reset(); }

void ScoreManager::reset() { totalScore = 0; }

void ScoreManager::add(int points) { totalScore += points; }

int ScoreManager::getTotal() const { return totalScore; }
