#ifndef aHighResTimerFILE
#define aHighResTimerFILE
#include <iostream>
#include <chrono>

//Written by Olivier Szymanezyk oszymanezyk@lincoln.ac.uk

class HighResTimer
{
private:
	typedef std::chrono::high_resolution_clock Clock;
	std::chrono::steady_clock::time_point startChrono;

public:
	HighResTimer() {};

	void resetChronoTimer()
	{
		startChrono = Clock::now();
	};

	float getChronoTime() 
	{
		std::chrono::steady_clock::time_point now = Clock::now();
		auto timeDiff = std::chrono::duration_cast<std::chrono::nanoseconds>(now - startChrono).count();
		return (float)timeDiff;
	};
};
#endif