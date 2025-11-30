#pragma once
#include <string>
#include <vector>

struct LeaderboardEntry {
    std::string name;
    int score;
    std::string difficulty;
    std::string date;
};

class Leaderboard {
public:
    Leaderboard(const std::string& filePath);
    void addEntry(const std::string& name, int score, const std::string& difficulty, const std::string& date);
    std::vector<LeaderboardEntry> getTop(int n);
    void display();

private:
    std::string filePath;
    std::vector<LeaderboardEntry> entries;
    void load();
    void save();
};
