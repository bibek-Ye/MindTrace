#include "Timer.h"

void Timer::sleep_ms(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

double Timer::elapsedSeconds(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end) {
    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
}
