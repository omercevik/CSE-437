/*
    CSE 437
    Real Time System Architectures
    Homework 02
    Omer CEVIK
    161044004
    main.cpp
*/
#include "gtu.hpp"

constexpr auto THREAD_SIZE = 20;

void thread_1_function(void);
void thread_2_function(void);

int main(void)
{
    srand(time(NULL));
    std::thread t[THREAD_SIZE];

    for (int i = 0; i < THREAD_SIZE; ++i)
        t[i] = rand()%2 ? std::thread(thread_1_function) : std::thread(thread_2_function);

    std::thread notScheduledThread(thread_1_function);

    for (int i = 0; i < THREAD_SIZE; ++i)
        gtu::setScheduleThread(t[i],i+1);

    gtu::startProtocol();

    for (int i = 0; i < THREAD_SIZE; ++i)
        t[i].join();

    notScheduledThread.join();

    return 0;
}
