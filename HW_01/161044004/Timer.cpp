/*
    CSE 437
    Real Time System Architectures
    Homework 01
    Timer.cpp
    Omer CEVIK
    161044004
*/
#include "ITimer.h"

class Timer : public ITimer
{
public:
    Timer(){}

    // run the callback once at time point tp.
    void registerTimer(const Timepoint &tp, const TTimerCallback &cb)
    {
        while(CLOCK::now() < tp);

        int start_time = CLOCK::now().time_since_epoch().count()/1000000;

        cb();

        int end_time = CLOCK::now().time_since_epoch().count()/1000000;

        int time_difference = (end_time - start_time);

        std::cout << "First Function : void registerTimer(const Timepoint &tp, const TTimerCallback &cb)\n"
                  << "Callback Start Time : " << start_time << " milli seconds." << std::endl
                  << "Callback End Time : " << end_time << " milli seconds." << std::endl
                  << "Time Difference : " <<  time_difference << " milli seconds." << std::endl;

        if (time_difference != 5)
            std::cerr << "Deadline error" << std::endl;
        std::cout << std::endl;
    }

    // run the callback periodically forever. The first call will be executed as soon as this callback is registered.
    void registerTimer(const Millisecs & period, const TTimerCallback &cb)
    {
        int periodCounter = 0;

        while(true)
        {
            Timepoint callBackPeriod = CLOCK::now() + period;
            while(CLOCK::now() < callBackPeriod);
            ++periodCounter;
            int start_time = CLOCK::now().time_since_epoch().count()/1000000;

            cb();

            int end_time = CLOCK::now().time_since_epoch().count()/1000000;

            int time_difference = (end_time - start_time);

            std::cout << "Second Function : void registerTimer(const Millisecs & period, const TTimerCallback &cb)" << std::endl
                      << periodCounter << ". Period Time : " << period.count() << " milli seconds." << std::endl
                      << "Callback Start Time : " << start_time << " milli seconds." << std::endl
                      << "Callback End Time : " << end_time << " milli seconds." << std::endl
                      << "Time Difference : " <<  time_difference << " milli seconds." << std::endl;

            if (time_difference != 5)
                std::cerr << "Deadline error" << std::endl;
            std::cout << std::endl;
        }
    }
    // Run the callback periodically until time point tp. The first call will be executed as soon as this callback is
    //registered.
    void registerTimer(const Timepoint &tp, const Millisecs & period, const TTimerCallback &cb)
    {
        int periodCounter = 0;
        while(CLOCK::now() < tp)
        {
            Timepoint callBackPeriod = CLOCK::now() + period;
            while(CLOCK::now() < callBackPeriod);
            ++periodCounter;

            int start_time = CLOCK::now().time_since_epoch().count()/1000000;

            cb();

            int end_time = CLOCK::now().time_since_epoch().count()/1000000;

            int time_difference = (end_time - start_time);

            std::cout << "Third Function : void registerTimer(const Timepoint &tp, const Millisecs & period, const TTimerCallback &cb)" << std::endl
                      << "Timepoint Time : " << tp.time_since_epoch().count()/1000000 << " milli seconds." << std::endl
                      << periodCounter << ". Period Time : " << period.count() << " milli seconds." << std::endl
                      << "Callback Start Time : " << start_time << " milli seconds." << std::endl
                      << "Callback End Time : " << end_time << " milli seconds." << std::endl
                      << "Time Difference : " <<  time_difference << " milli seconds." << std::endl;

            if (time_difference != 5)
                std::cerr << "Deadline error" << std::endl;
            std::cout << std::endl;
        }
    }
    // Run the callback periodically. Before calling the callback every time, call the predicate first to check if the
    //termination criterion is satisfied. If the predicate returns false, stop calling the callback.
    void registerTimer(const TPredicate &pred, const Millisecs & period, const TTimerCallback &cb)
    {
        int periodCounter = 0;
        while( pred() )
        {
            Timepoint callBackPeriod = CLOCK::now() + period;
            while(CLOCK::now() < callBackPeriod);
            ++periodCounter;

            int start_time = CLOCK::now().time_since_epoch().count()/1000000;

            cb();

            int end_time = CLOCK::now().time_since_epoch().count()/1000000;

            int time_difference = (end_time - start_time);

            std::cout << "Fourth Function : void registerTimer(const TPredicate &pred, const Millisecs & period, const TTimerCallback &cb)" << std::endl
                      <<  "pred() method called true!" << std::endl
                      << periodCounter << ". Period Time : " << period.count() << " milli seconds." << std::endl
                      << "Callback Start Time : " << start_time << " milli seconds." << std::endl
                      << "Callback End Time : " << end_time << " milli seconds." << std::endl
                      << "Time Difference : " <<  time_difference << " milli seconds." << std::endl;

            if (time_difference != 5)
                std::cerr << "Deadline error" << std::endl;
            std::cout << std::endl;
        }
        std::cout << "Fourth Function : void registerTimer(const TPredicate &pred, const Millisecs & period, const TTimerCallback &cb)" << std::endl
                  <<  "pred() method called false!" << std::endl << std::endl;
    }
};
