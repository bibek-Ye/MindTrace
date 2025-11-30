#include "Leaderboard.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

Leaderboard::Leaderboard(const std::string& path) : filePath(path) { load(); }

void Leaderboard::load() {
    entries.clear();
    std::ifstream fin(filePath);
    if (!fin.is_open()) return;

    std::string line;
    while (std::getline(fin, line)) {
        std::istringstream ss(line);
        std::string name, diff, date;
        int score;
        if (ss >> name >> score >> diff >> date)
            entries.push_back({name, score, diff, date});
    }
}

void Leaderboard::save() {
    std::ofstream fout(filePath, std::ios::trunc);
    for (auto &e : entries)
        fout << e.name << " " << e.score << " " << e.difficulty << " " << e.date << "\n";
}

void Leaderboard::addEntry(const std::string& name, int score, const std::string& difficulty, const std::string& date) {
    entries.push_back({name, score, difficulty, date});
    std::sort(entries.begin(), entries.end(), [](auto &a, auto &b){ return b.score > a.score; });
    save();
}

std::vector<LeaderboardEntry> Leaderboard::getTop(int n) {
    if (n > (int)entries.size()) n = entries.size();
    return std::vector<LeaderboardEntry>(entries.begin(), entries.begin() + n);
}

void Leaderboard::display() {
    std::cout << "======= LEADERBOARD =======\n";
    auto top = getTop(5);
    for (auto &e : top) {
        std::cout << e.name << " | " << e.score << " | " << e.difficulty << " | " << e.date << "\n";
    }
    if (top.empty()) std::cout << "No entries yet.\n";
}
