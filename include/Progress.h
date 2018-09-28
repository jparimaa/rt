#pragma once

#include <unordered_map>
#include <thread>

class Progress
{
public:
    Progress(){};
    void setTotalProgress(int totalProgress);
    void increaseProgressForThread(std::thread::id thread);
    float getProgressPercentage();
    bool completed();

private:
    int m_totalProgress = 0;
    int m_currentProgress = 0;
    std::unordered_map<std::thread::id, int> m_progressPerThread;
    bool m_updateRequired = false;

    void updateCurrentProgress();
};