#pragma once

#include <chrono>
#include <ctime>
namespace basicgraphics {

	class Clock
	{
	public:
		Clock();
		~Clock();

		std::chrono::milliseconds tick(); // provides time since last tick

	protected:
		bool firstTick;
		std::chrono::time_point<std::chrono::steady_clock> time;
	};


}