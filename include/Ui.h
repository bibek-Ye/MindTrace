#pragma once
#include <string>

class UI {
public:
    void clearScreen();
    void moveCursor(int x, int y);
    void printCentered(int y, const std::string& text);
    void printAsciiCentered(int y, const std::string& asciiArt); // NEW: ASCII art
    void drawDivider(int y);
    void printHeader(const std::string& title, int score, int round, const std::string& diff);
    void showCountdownTopRight(int remaining);
    void beepCorrect();
    void beepWrong();
    void resetColor();
    void setColor(int attr);
    void waitForKey();
    int getConsoleWidth();
    int getConsoleHeight();
    void setConsoleSize(int width, int height);
};
