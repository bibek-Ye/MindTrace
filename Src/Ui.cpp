#include "UI.h"
#include <iostream>
#include <conio.h>
#include <windows.h>

void UI::clearScreen() { system("cls"); }

void UI::moveCursor(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int UI::getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

int UI::getConsoleHeight() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void UI::setConsoleSize(int width, int height) {
    std::string cmd = "mode con: cols=" + std::to_string(width) + " lines=" + std::to_string(height);
    system(cmd.c_str());
}

void UI::printCentered(int y, const std::string& text) {
    int width = getConsoleWidth();
    int x = (width - text.size()) / 2;
    moveCursor(x, y);
    std::cout << text;
}

// NEW: Print ASCII art centered
void UI::printAsciiCentered(int y, const std::string& asciiArt) {
    int width = getConsoleWidth();
    int lineY = y;
    size_t start = 0, end;
    while ((end = asciiArt.find('\n', start)) != std::string::npos) {
        std::string line = asciiArt.substr(start, end - start);
        int x = (width - line.size()) / 2;
        moveCursor(x, lineY++);
        std::cout << line;
        start = end + 1;
    }
    // Print last line
    std::string lastLine = asciiArt.substr(start);
    int x = (width - lastLine.size()) / 2;
    moveCursor(x, lineY);
    std::cout << lastLine;
}

void UI::drawDivider(int y) {
    int width = getConsoleWidth();
    moveCursor(0, y);
    for (int i = 0; i < width; i++) std::cout << "-";
    std::cout << "\n";
}

void UI::printHeader(const std::string& title, int score, int round, const std::string& diff) {
    clearScreen();
    int width = getConsoleWidth();
    std::string header = title + " | Score: " + std::to_string(score) +
                         " | Round: " + std::to_string(round) + " | " + diff;
    printCentered(0, header);
}

void UI::showCountdownTopRight(int remaining) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    // Save cursor position
    COORD savedPos = csbi.dwCursorPosition;

    // Move to top-right
    int width = getConsoleWidth();
    moveCursor(width - 15, 0);

    // Flash red if remaining <= 3 seconds
    if (remaining <= 3) setColor(12); // red
    else setColor(7); // white

    std::cout << "Time: " << remaining << "s ";
    std::cout.flush();
    resetColor();

    // Restore cursor
    SetConsoleCursorPosition(hConsole, savedPos);
}

void UI::beepCorrect() { Beep(1000, 150); }
void UI::beepWrong()   { Beep(400, 400); }
void UI::resetColor() { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); }
void UI::setColor(int attr) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attr); }
void UI::waitForKey() { _getch(); }
