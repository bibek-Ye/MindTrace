#pragma once
#include "SequenceGenerator.h"
#include "Timer.h"
#include "ScoreManager.h"
#include "Leaderboard.h"
#include "UI.h"

#include <string>

class Game {
public:
    Game();
    void start();

private:
    SequenceGenerator seqGen;
    Timer timer;
    ScoreManager scoreMgr;
    Leaderboard leaderboard;
    UI ui;

    int round;
    int seqLength;
    Difficulty difficulty;

    int startingLength;
    int growthPerRound;
    int displaySpeedMs;
    int inputTimeLimitSec;

    std::string playerName;

    void configureDifficulty();
    void mainMenu();
    void playSession();
    bool playRound();  // returns true to continue, false to stop session
    bool getPlayerInputWithin(const std::string& prompt, std::string& out, int timeLimitSec, double& timeTakenSec);
    std::string difficultyToString() const;
    std::string currentDateString() const;
};
