#include "Progress.h"

void Progress::setTotalProgress(int totalProgress)
{
    m_totalProgress = totalProgress;
}

void Progress::increaseProgressForThread(std::thread::id thread)
{
    ++m_progressPerThread[thread];
    m_updateRequired = true;
}

float Progress::getProgressPercentage()
{
    updateCurrentProgress();
    return static_cast<float>(m_currentProgress) / static_cast<float>(m_totalProgress);
}

bool Progress::completed()
{
    updateCurrentProgress();
    return m_currentProgress >= m_totalProgress;
}

void Progress::updateCurrentProgress()
{
    if (!m_updateRequired)
    {
        return;
    }
    m_currentProgress = 0;
    for (auto& kv : m_progressPerThread)
    {
        m_currentProgress += kv.second;
    }
}