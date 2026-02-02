#pragma once
#include <chrono>
#include <vector>
#include <iostream>

typedef std::chrono::steady_clock timer_clock;

// Wrapper object for a timer 

class Timer 
{
    public: 
        long long delta = 0;  // time accumulated  
        int num_samples = 0;  // how many measurement samples
        timer_clock::time_point now;
        const char* name;

        Timer(const char* name) : name(name){ now = timer_clock::now(); };

        void start(){ now = timer_clock::now(); };
        void stop(){ 
            auto dt = timer_clock::now() - now; 
            delta += std::chrono::duration_cast<std::chrono::microseconds>(dt).count();
            num_samples++;
        };
        float get_mean_duration(){
            if (!num_samples) {return 0.0;}
            return 1.0 * delta/ 1000.0 / num_samples;
        };
        void print_end_results(){
            float duration = get_mean_duration();
            printf("Stage: %s    Samples: %i,     Mean Duration: %f ms \n\n", name, num_samples, duration);
        };
};