#pragma once

#include <chrono>
#include <stdint.h>

class Clock {
    public:
        Clock() {}
        ~Clock() {}
        void Init();
        void Restart();
        double GetInitTime();
        double GetElapsedTime();
        void Pause();
        void Play();
        void ResetPlayPause();
        bool IsPaused() { return clockPaused; };

    
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> initTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;
        std::chrono::duration<double> elapsedTime;

        std::chrono::time_point<std::chrono::high_resolution_clock> initPauseTime;
        std::chrono::duration<double> pausedTime;
        
        bool clockPaused = false;
};