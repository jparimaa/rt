#include "Stopwatch.h"

void Stopwatch::start()
{
    m_startTime = std::chrono::high_resolution_clock::now();
}

size_t Stopwatch::getTimeSinceStart()
{
    auto timeSinceStart = std::chrono::high_resolution_clock::now() - m_startTime;
    return std::chrono::duration_cast<std::chrono::seconds>(timeSinceStart).count();
}
