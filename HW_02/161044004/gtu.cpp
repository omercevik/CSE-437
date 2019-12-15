/*
    CSE 437
    Real Time System Architectures
    Homework 02
    Omer CEVIK
    161044004
    gtu.cpp
*/
#include "gtu.hpp"

void gtu::mutex::lock()
{
    if (!containsThread(std::this_thread::get_id()))
    {
        std::cerr << "\nLock Error! Thread has not been registered! : " << std::this_thread::get_id() << std::endl << std::endl;
    }
    else
    {
        gtu::mutex* acquiredMutex = getAcquiredMutex();

        if(acquiredMutex != nullptr)
        {
            if (this->getThreadPriority() > acquiredMutex->getCeil())
            {
                std::mutex::lock();
                this->setAcquired(true);
            }
            else
            {
                std::unique_lock<std::mutex> conditionLock(lockMutex);
                lockingConditionVariable.wait(conditionLock,[&]{ acquiredMutex = getAcquiredMutex(); return acquiredMutex == nullptr ? true : false; });
                std::mutex::lock();
                this->setAcquired(true);
            }
        }
        else
        {
            std::mutex::lock();
            this->setAcquired(true);
        }
    }

    std::cout << "Locked thread id : " << std::this_thread::get_id() << std::endl << std::endl;
}

void gtu::mutex::unlock()
{
    if (!containsThread(std::this_thread::get_id()))
    {
        std::cerr << "\nUnlock Error! Thread has not been registered! : " << std::this_thread::get_id() << std::endl << std::endl;
    }
    else
    {
        for (int i = 0; i < this->registered_threads.size(); ++i)
            if (this->registered_threads[i].getRegisteredThreadId() == std::this_thread::get_id())
                this->registered_threads.erase(this->registered_threads.begin() + i);

        int maxPriority = this->getMaxPriority();
        this->setCeil(maxPriority);

        lockingConditionVariable.notify_one();

        std::mutex::unlock();
        std::cout << "Unlocked thread id : " << std::this_thread::get_id() << std::endl << std::endl;
        this->setAcquired(false);
    }
}

const bool gtu::mutex::containsThread(const std::thread::id newThreadId) const
{
    for (int i = 0; i < this->registered_threads.size(); ++i)
        if (this->registered_threads[i].getRegisteredThreadId() == newThreadId)
            return true;
    return false;
}

const int gtu::mutex::getMaxPriority(void) const
{
    int maxPriority = !this->registered_threads.empty() ? this->registered_threads[0].getRegisteredPriority() : -std::numeric_limits<int>::infinity();;

    for (int i = 1; i < this->registered_threads.size(); ++i)
        if (this->registered_threads[i].getRegisteredPriority() > maxPriority)
            maxPriority = this->registered_threads[i].getRegisteredPriority();
    return maxPriority;
}

void gtu::mutex::registerThreadToMutex(const std::thread& newThread)
{
	gtu::RegThreadInfo reg_thread;
	reg_thread.setRegisteredThreadId(newThread.get_id());

	#ifdef __linux__
		// Linux scheduler for priority.

		sched_param schedulerPriority;
		int policy;
		pthread_getschedparam(pthread_self(), &policy, &schedulerPriority);
		reg_thread.setRegisteredPriority(schedulerPriority.sched_priority);

	#elif _WIN32
		// Windows scheduler for priority

		reg_thread.setRegisteredPriority(GetThreadPriority(GetCurrentThread()));

	#endif

    reg_thread.addRegisteredMutexes(this);
    this->registered_threads.push_back(reg_thread);

    int maxPriority = getMaxPriority();
    this->setCeil(maxPriority);
}

gtu::mutex* gtu::mutex::getAcquiredMutex(void) const
{
    for (int i = 0; i < this->registered_threads.size(); ++i)
        if (this->registered_threads[i].getRegisteredThreadId() == std::this_thread::get_id())
            for (int j = 0; j < this->registered_threads[i].getRegisteredMutexes().size(); ++j)
                if (this->registered_threads[i].getRegisteredMutexes()[j]->getAcquired())
                    return this->registered_threads[i].getRegisteredMutexes()[j];
    return nullptr;
}

const int gtu::mutex::getThreadPriority(void) const
{
    for (int i = 0; i < this->registered_threads.size(); ++i)
        if (this->registered_threads[i].getRegisteredThreadId() == std::this_thread::get_id())
            return this->registered_threads[i].getRegisteredPriority();
    return -1;
}

int increment = 0;
bool condition = false;

#ifdef __linux__
// Linux priority prints.

sched_param schedulingPriority;

#endif

std::condition_variable conditionVariableOfFunction;

std::mutex conditionVariableMutexOfFunctions;
gtu::mutex mutex1;
gtu::mutex mutex2;
gtu::mutex mutex3;

void gtu::startProtocol(void)
{
    condition = true;
    conditionVariableOfFunction.notify_all();
}

void gtu::setScheduleThread(std::thread& thisThread, int priorityOfThread)
{
	#ifdef __linux__
    // Linux priority prints.

	sched_param schedulingPriorityOfThread;
	schedulingPriorityOfThread.sched_priority = priorityOfThread;
	if(pthread_setschedparam(thisThread.native_handle(), SCHED_RR, &schedulingPriorityOfThread))
		std::cerr << "\nError! Thread can not be scheduled!" << std::endl << std::endl;

	#elif _WIN32
    // Windows priority prints.

	SetThreadPriority(GetCurrentThread(), priorityOfThread);
	#endif

    mutex1.registerThreadToMutex(thisThread);
    mutex2.registerThreadToMutex(thisThread);
    mutex3.registerThreadToMutex(thisThread);
}

void thread_1_function(void)
{
    std::unique_lock<std::mutex> lockThreadFunction1(conditionVariableMutexOfFunctions);
    conditionVariableOfFunction.wait(lockThreadFunction1, [&]{ return condition; });

    const std::unique_lock<gtu::mutex> lock1(mutex1);
    const std::lock_guard<gtu::mutex> lock2(mutex2);
    const std::lock_guard<gtu::mutex> lock3(mutex3);

    ++increment;

	#ifdef __linux__
	// Linux priority prints.

	int policy;
    pthread_getschedparam(pthread_self(), &policy, &schedulingPriority);
    std::cout << "First Function Priority  value is :\t" << schedulingPriority.sched_priority << std::endl
			  << "First Function increased value is :\t" << increment << std::endl << std::endl;

	#elif _WIN32
	// Windows priority prints.

	std::cout << "First Function Priority  value is :\t" << GetCurrentThreadId() << std::endl
			  << "First Function increased value is :\t" << increment << std::endl << std::endl;
	#endif
}

void thread_2_function(void)
{
    std::unique_lock<std::mutex> lockThreadFunction2(conditionVariableMutexOfFunctions);
    conditionVariableOfFunction.wait(lockThreadFunction2, [&]{ return condition;});

    const std::lock_guard<gtu::mutex> lock3(mutex3);
    const std::lock_guard<gtu::mutex> lock2(mutex2);
    const std::unique_lock<gtu::mutex> lock1(mutex1);

	++increment;

	#ifdef __linux__
	// Linux priority prints.

	int policy;
	pthread_getschedparam(pthread_self(), &policy, &schedulingPriority);
	std::cout << "Second Function Priority  value is :\t" << schedulingPriority.sched_priority << std::endl
			  << "Second Function increased value is :\t" << increment << std::endl << std::endl;

	#elif _WIN32
	// Windows priority prints.

	std::cout << "Second Function Priority  value is :\t" << GetCurrentThreadId() << std::endl
			  << "Second Function increased value is :\t" << increment << std::endl << std::endl;
	#endif
}
