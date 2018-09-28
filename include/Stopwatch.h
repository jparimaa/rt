#pragma once

#include <chrono>

class Stopwatch
{
public:
    Stopwatch(){};
    void start();
    size_t getTimeSinceStart();

private:
    std::chrono::time_point<std::chrono::steady_clock> m_startTime;
};