#include "Clock.hpp"

void Clock::Init() 
{
    initTime = std::chrono::high_resolution_clock::now();
    lastTime = initTime;

    ResetPlayPause();
}

void Clock::Restart()
{
    lastTime = std::chrono::high_resolution_clock::now();

    ResetPlayPause();
}

double Clock::GetInitTime() 
{
    double r = initTime.time_since_epoch().count();
    return r;
}

double Clock::GetElapsedTime() 
{
    elapsedTime = std::chrono::high_resolution_clock::now() - lastTime;

    if(clockPaused)
    {
        elapsedTime = initPauseTime - lastTime;
    }

    elapsedTime -= pausedTime;    
    
    return elapsedTime.count();
}

void Clock::Pause()
{
    initPauseTime = std::chrono::high_resolution_clock::now();
    clockPaused = true;
}

void Clock::Play()
{
    if(clockPaused)
    {
        pausedTime += std::chrono::high_resolution_clock::now() - initPauseTime;
        clockPaused = false;
    }
}

void Clock::ResetPlayPause()
{
    initPauseTime = std::chrono::high_resolution_clock::now();
    pausedTime = initPauseTime - initPauseTime;
    clockPaused = false;
}
