#include "SequenceGenerator.h"
#include <random>

std::string SequenceGenerator::generate(int length) {
    std::string seq;
    std::string chars = "abcdefghijklmnopqrstuvwxyz0123456789@#$&*"; // lowercase + numbers + symbols
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, chars.size() - 1);

    for (int i = 0; i < length; ++i)
        seq += chars[dis(gen)];

    return seq;
}
