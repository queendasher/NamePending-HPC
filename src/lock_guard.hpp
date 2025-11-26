#ifndef LOCKGUARD_H
#define LOCKGUARD_H

#include <atomic>
#include <thread>

namespace ASC_HPC {
    class LockGuard
    {
    public:
        std::atomic<bool>& mtx_;

        LockGuard(std::atomic<bool>& mtx) 
        : mtx_(mtx)
        {
            bool expected = false;
            // Spin until we successfully set mtx_ to true
            while (!mtx_.compare_exchange_strong(expected, true))
            {
                expected = false; // reset expected for the next iteration

                // Optionally, add a pause or yield here to reduce contention
                std::this_thread::yield();
            }
        }

        ~LockGuard()
        {
            mtx_.store(false); // Release the lock
        }
    };
} // namespace ASC_HPC

#endif // LOCKGUARD_H