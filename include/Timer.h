#pragma once
#include <chrono>
#include <thread>

class Timer {
public:
    void sleep_ms(int ms);
    double elapsedSeconds(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end);
};
