#pragma once
#include <chrono>

class FunctionProfiler
{
    std::chrono::high_resolution_clock::time_point t;
public:
    /**
     * @brief  Timer for tracking of duration of functions.
     * @note   
     * @retval 
     */
    FunctionProfiler() {}

    /**
     * @brief Starts the timer
     * @note 
     * @retval
     */
    void start() {
        t = std::chrono::high_resolution_clock::now();
    }

    /**
     * @brief  Returns time in ms since creation of object
     * @note   
     * @retval time in ms
     */
    double stop()
    {
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> dur = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t);
        return dur.count();
    }
};
