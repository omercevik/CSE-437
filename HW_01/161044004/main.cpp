/*
    CSE 437
    Real Time System Architectures
    Homework 01
    main.cpp
    Omer CEVIK
    161044004
*/
#include "Timer.cpp"

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    const Millisecs period{200};

    const TTimerCallback cb = [&]
    {
        Millisecs ms{5};
        Timepoint end = CLOCK::now() + ms;
        while(CLOCK::now() < end);
    };

    const TPredicate pred = [&]
    {
        return (rand() % 10) > 5;
    };

    const Timepoint tp = CLOCK::now();

    Timer time;


    std::thread t1([&]
    {
        time.registerTimer(tp,cb);
    });

    std::thread t2([&]
    {
       time.registerTimer(period,cb);
    });

    std::thread t3([&]
    {
        time.registerTimer(tp + 5*period,period,cb);
    });

    std::thread t4([&]
    {
        time.registerTimer(pred,period,cb);
    });

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
