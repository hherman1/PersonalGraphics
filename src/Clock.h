#pragma once

#include <ctime>
#include <GLFW/glfw3.h>

//Originally implemented with <chrono>'s std::steady_clock
//However, that clock is very inconsistent and causes very odd rendering. Avoid it!
// Uses glfwGetTime() instead.
namespace basicgraphics {

	class Clock
	{
	public:
		Clock();
		~Clock();

		double tick(); // provides time since last tick

	protected:
		bool firstTick;
		double time; // use glfwGetTime() instead
		
	};


}
