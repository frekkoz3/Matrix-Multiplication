#include "Timer.hpp"

void Timer::start()
{
    start_ = std::chrono::high_resolution_clock::now();
}

double Timer::stopMilliseconds()
{
    const auto end =
        std::chrono::high_resolution_clock::now();

    const std::chrono::duration<double, std::milli> elapsed =
        end - start_;

    return elapsed.count();
}