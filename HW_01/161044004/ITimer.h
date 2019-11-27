/*
    CSE 437
    Real Time System Architectures
    Homework 01
    ITimer.h
    Omer CEVIK
    161044004
*/
#include <iostream>
#include <chrono>
#include <functional>
#include <thread>
#include <cstdlib>
#include <ctime>

using CLOCK = std::chrono::high_resolution_clock;
using TTimerCallback = std::function<void()>;
using Millisecs = std::chrono::milliseconds;
using Timepoint = CLOCK::time_point;
using TPredicate = std::function<bool()>;


class ITimer {
public:
// run the callback once at time point tp.
virtual void registerTimer(const Timepoint &tp, const TTimerCallback &cb) = 0;
// run the callback periodically forever. The first call will be executed as soon as this callback is registered.
virtual void registerTimer(const Millisecs & period, const TTimerCallback &cb) = 0;
// Run the callback periodically until time point tp. The first call will be executed as soon as this callback is
//registered.
virtual void registerTimer(const Timepoint &tp, const Millisecs & period, const TTimerCallback &cb) = 0;
// Run the callback periodically. Before calling the callback every time, call the predicate first to check if the
//termination criterion is satisfied. If the predicate returns false, stop calling the callback.
virtual void registerTimer(const TPredicate &pred, const Millisecs & period, const TTimerCallback &cb) = 0;
};
