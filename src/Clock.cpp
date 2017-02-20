#include "Clock.h"


namespace basicgraphics {

	Clock::Clock() :
		time(std::chrono::steady_clock::now()),
		firstTick(true)
	{
	}


	Clock::~Clock()
	{
	}
	std::chrono::milliseconds Clock::tick()
	{
		auto old = time;
		time = std::chrono::steady_clock::now();
		if (firstTick) {
			firstTick = false;
			return std::chrono::milliseconds(0);
		}
		else {
			return std::chrono::duration_cast<std::chrono::milliseconds>(time - old);
		}
	}
}
