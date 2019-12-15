/*
    CSE 437
    Real Time System Architectures
    Homework 02
    Omer CEVIK
    161044004
    gtu.hpp
*/
#ifndef GTU_HPP
#define GTU_HPP

#include <iostream>
#include <mutex>
#include <chrono>
#include <vector>
#include <condition_variable>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <limits>

#ifdef _WIN32
// Windows libraries.
#include <Windows.h>
#include <process.h>

#endif

namespace gtu
{
    void startProtocol();
    void setScheduleThread(std::thread& thisThread, int priorityOfThread);

    class RegThreadInfo;

    class mutex : public std::mutex
    {
        public:
            void lock();
            void unlock();
            void setCeil(const int newCeil){ this->ceil = newCeil; }
            void setAcquired(const bool newAcquired){ this->isAcquired = newAcquired; }
            void registerThreadToMutex(const std::thread& newThread);
            const bool containsThread(const std::thread::id newThreadId) const;
            const bool getAcquired(void) const { return this->isAcquired; }
            const int getCeil(void) const { return this->ceil; }
            const int getMaxPriority(void) const;
            const int getThreadPriority(void) const;
            gtu::mutex* getAcquiredMutex(void) const;

        private:
            bool isAcquired = false;
            int ceil = -1;
            std::condition_variable lockingConditionVariable;
            std::mutex lockMutex;
            std::vector<RegThreadInfo> registered_threads;
    };

    class RegThreadInfo
    {
    public:
        const int getRegisteredPriority(void)const { return this->reg_priority; }
        const std::thread::id getRegisteredThreadId(void)const { return this->reg_thread_id; }
        const std::vector<gtu::mutex*> getRegisteredMutexes(void)const { return this->reg_mutexes; }

        void setRegisteredPriority(const int newPriority) { this->reg_priority = newPriority; }
        void setRegisteredThreadId(const std::thread::id newThreadId) { this->reg_thread_id = newThreadId; }
        void addRegisteredMutexes(gtu::mutex* newMutex) { this->reg_mutexes.push_back(newMutex); }

    private:
        int reg_priority;
        std::thread::id reg_thread_id;
        std::vector<gtu::mutex*> reg_mutexes;
    };
}

#endif
