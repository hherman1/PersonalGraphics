#include "Clock.h"


namespace basicgraphics {

	Clock::Clock() :
		time(glfwGetTime()),
		firstTick(true)
	{
	}


	Clock::~Clock()
	{
	}
	double Clock::tick()
	{
		auto old = time;
		time = glfwGetTime();
		if (firstTick) {
			firstTick = false;
			return 0;
		}
		else {
			return time - old;
		}
	}
}
