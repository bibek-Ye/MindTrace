#include "Game.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <conio.h>

using namespace std;

Game::Game()
    : round(1), seqLength(4), difficulty(Difficulty::EASY),
      leaderboard("data/leaderboard.txt")
{
    startingLength = 4;
    growthPerRound = 1;
    displaySpeedMs = 500;
    inputTimeLimitSec = 10; // default for Easy

    ui.setConsoleSize(120, 30); // wider console for ASCII
}

void Game::start() { mainMenu(); }

// ---------- Interactive Main Menu with corrected ASCII ----------
void Game::mainMenu() {
    const int menuSize = 3;
    string options[menuSize] = { "Play Game", "View Leaderboard", "Exit" };
    int choice = 0;

std::string titleAscii = R"(
   __  __ _____ _   _ _____ _______ _____            _____ ______ 
 |  \/  |_   _| \ | |  __ \__   __|  __ \     /\   / ____|  ____|
 | \  / | | | |  \| | |  | | | |  | |__) |   /  \ | |    | |__   
 | |\/| | | | | . ` | |  | | | |  |  _  /   / /\ \| |    |  __|  
 | |  | |_| |_| |\  | |__| | | |  | | \ \  / ____ \ |____| |____ 
 |_|  |_|_____|_| \_|_____/  |_|  |_|  \_\/_/    \_\_____|______|
                                                                                                                 
                                                        
)";


    while (true) {
        ui.clearScreen();
        ui.printAsciiCentered(2, titleAscii);

        // Display menu with highlight
        for (int i = 0; i < menuSize; i++) {
            if (i == choice) ui.setColor(11); // cyan highlight
            else ui.resetColor();
            ui.printCentered(10 + i, options[i]);
        }
        ui.resetColor();

        int key = _getch();
        if (key == 224) key = _getch(); // arrow keys

        if (key == 72) choice--; // UP
        else if (key == 80) choice++; // DOWN

        if (choice < 0) choice = menuSize - 1;
        else if (choice >= menuSize) choice = 0;

        if (key == '\r') { // Enter
            if (choice == 0) { // Play Game
                ui.clearScreen();
                ui.printCentered(5, "Enter your name: ");
                ui.moveCursor((ui.getConsoleWidth() - 20)/2, 7);
                getline(cin, playerName);
                if (playerName.empty()) playerName = "Player";

                configureDifficulty();
                playSession();
            }
            else if (choice == 1) { // Leaderboard
                ui.clearScreen();
                ui.setColor(11);
                ui.printCentered(2, "======= LEADERBOARD =======");
                ui.resetColor();
                leaderboard.display();
                cout << "\nPress any key to return...";
                ui.waitForKey();
            }
            else if (choice == 2) return; // Exit
        }
    }
}

// ---------- Interactive Difficulty Menu (plain text) ----------
void Game::configureDifficulty() {
    ui.clearScreen();

    // Plain text title instead of ASCII
    ui.printCentered(2, "SELECT DIFFICULTY LEVEL");

    string options[3] = { "EASY   - Relaxed pace", "MEDIUM - Moderate challenge", "HARD   - High difficulty" };
    int choice = 0;

    while (true) {
        for (int i = 0; i < 3; i++) {
            if (i == choice) ui.setColor(11); // cyan highlight
            else ui.resetColor();
            ui.printCentered(4 + i, options[i]); // starting row = 4
        }
        ui.resetColor();

        int key = _getch();
        if (key == 224) key = _getch();

        if (key == 72) choice--; // UP
        else if (key == 80) choice++; // DOWN

        if (choice < 0) choice = 2;
        else if (choice > 2) choice = 0;

        if (key == '\r') { // Enter
            if (choice == 0) { // EASY
                difficulty = Difficulty::EASY;
                startingLength = 4; growthPerRound = 1; displaySpeedMs = 500; inputTimeLimitSec = 10;
            }
            else if (choice == 1) { // MEDIUM
                difficulty = Difficulty::MEDIUM;
                startingLength = 6; growthPerRound = 2; displaySpeedMs = 400; inputTimeLimitSec = 20;
            }
            else { // HARD
                difficulty = Difficulty::HARD;
                startingLength = 8; growthPerRound = 2; displaySpeedMs = 300; inputTimeLimitSec = 30;
            }
            break;
        }
    }

    seqLength = startingLength;
    scoreMgr.reset();
    round = 1;
}

// ---------- Game session ----------
void Game::playSession() {
    round = 1; 
    seqLength = startingLength;
    bool continueGame = true;

    while (continueGame) {
        continueGame = playRound();
    }
}

// ---------- Play a single round ----------
bool Game::playRound() {
    ui.clearScreen();
    ui.printHeader("MINDTRACE - " + playerName, scoreMgr.getTotal(), round, difficultyToString());
    ui.printCentered(2, "Memorize the sequence:");

    string seq = seqGen.generate(seqLength);

    int width = ui.getConsoleWidth();
    int startX = (width - seq.length() * 2)/2;
    int y = 4;

    for (int i = 0; i < seq.length(); i++) {
        ui.moveCursor(startX + i*2, y);
        ui.setColor(9);
        cout << seq[i];
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(displaySpeedMs));
    }

    for (int i = 0; i < seq.length(); i++) {
        ui.moveCursor(startX + i*2, y);
        cout << " ";
    }
    ui.resetColor();

    ui.moveCursor(0, y + 2);
    cout << "Enter the sequence (" << seqLength << " chars) and press Enter:" << endl;
    cout << "> ";

    string userAns;
    double timeTaken = 0;

    bool ok = getPlayerInputWithin("", userAns, inputTimeLimitSec, timeTaken);

    if (!ok) {
        ui.clearScreen();
        cout << "Time's up!\nCorrect sequence was: " << seq << "\n";
        ui.beepWrong();
        leaderboard.addEntry(playerName, round-1, difficultyToString(), currentDateString());
        cout << "Press any key to return to main menu...";
        ui.waitForKey();
        return false;
    }

    if (userAns == seq) {
        cout << "\n✔ Correct!\n";
        ui.beepCorrect();

        int points = (difficulty == Difficulty::EASY) ? 4 :
                     (difficulty == Difficulty::MEDIUM) ? 6 : 8;
        scoreMgr.add(points);

        round++;
        seqLength += growthPerRound;

        cout << "Press any key to continue to next round...";
        ui.waitForKey();
        return true;
    } 
    else {
        cout << "\nWrong!\nCorrect sequence was: " << seq << "\n";
        ui.beepWrong();
        leaderboard.addEntry(playerName, round-1, difficultyToString(), currentDateString());

        cout << "Press any key to return to main menu...";
        ui.waitForKey();
        return false;
    }
}

// ---------- Live timer input ----------
bool Game::getPlayerInputWithin(const std::string& prompt, std::string& out, int timeLimitSec, double& timeTakenSec) {
    out = "";
    auto start = chrono::high_resolution_clock::now();
    int elapsed = 0;
    int lastShown = -1;

    if (!prompt.empty()) cout << prompt << endl;
    cout << "> ";

    while (elapsed < timeLimitSec) {
        int remaining = timeLimitSec - elapsed;
        if (remaining != lastShown) {
            ui.showCountdownTopRight(remaining);
            lastShown = remaining;
        }

        if (_kbhit()) {
            char c = _getch();
            if (c == '\r') break;
            if (c == '\b' && !out.empty()) { out.pop_back(); cout << "\b \b"; }
            else if (c >= 32 && c <= 126) { out += c; cout << c; }
        }

        this_thread::sleep_for(chrono::milliseconds(50));
        auto now = chrono::high_resolution_clock::now();
        elapsed = static_cast<int>(chrono::duration<double>(now - start).count());
    }

    auto end = chrono::high_resolution_clock::now();
    timeTakenSec = chrono::duration<double>(end - start).count();
    cout << endl;

    return !out.empty() && timeTakenSec <= timeLimitSec;
}

// ---------- Helper functions ----------
std::string Game::difficultyToString() const {
    switch(difficulty) {
        case Difficulty::EASY: return "EASY";
        case Difficulty::MEDIUM: return "MEDIUM";
        case Difficulty::HARD: return "HARD";
    }
    return "UNKNOWN";
}

std::string Game::currentDateString() const {
    time_t now = time(nullptr);
    tm* l = localtime(&now);
    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%d", l);
    return buf;
}