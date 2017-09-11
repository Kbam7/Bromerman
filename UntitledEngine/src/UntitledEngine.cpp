#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "UntitledEngine.h"
#include "UntitledEngineErrors.h"

namespace UntitledEngine {

	const float ONE_NANOSEC = 1000000000.0f; // One nano second

	int init() {

		// Initialise GLFW
		if (!glfwInit()) {
			fatalError("Failed to initialize GLFW\n");
			return -1;
		}

		// Set window hints for anti-aliasing and OpenGL version
		glfwWindowHint(GLFW_SAMPLES, 4);                        // 4x antialiasing
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);          // We want OpenGL 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);    // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

		// Init game time
		getGameTicks();

		return 0;
	}

	// gets time and stores it into 'ts'
	void current_utc_time(struct timespec *ts) {
		#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
			clock_serv_t cclock;
			mach_timespec_t mts;

			host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
			clock_get_time(cclock, &mts);
			mach_port_deallocate(mach_task_self(), cclock);
			ts->tv_sec = mts.tv_sec;
			ts->tv_nsec = mts.tv_nsec;
		#else
			clock_gettime(CLOCK_REALTIME, ts);
		#endif

	}

	// Returns time in milliseconds since this function was first called.
	// Used on init to game
	size_t  getGameTicks(void) {

		static struct timespec  _start_time = {-1,-1};  // time of init
		struct timespec         current_time;           // time now
		struct timespec         elapsed;                // time since init

		if (_start_time.tv_sec == -1) // init
			current_utc_time(&_start_time);
		//printf("start: %d.%09ld\n", (int)_start_time.tv_sec, _start_time.tv_nsec); // debug

		current_utc_time(&current_time);
		//printf("current: %d.%09ld\n", (int)current_time.tv_sec, current_time.tv_nsec); // debug

		elapsed.tv_sec = current_time.tv_sec - _start_time.tv_sec;
		if(current_time.tv_nsec >= _start_time.tv_nsec)
			elapsed.tv_nsec = current_time.tv_nsec - _start_time.tv_nsec;
		else    {
			elapsed.tv_sec--;
			elapsed.tv_nsec = (long)ONE_NANOSEC + current_time.tv_nsec - _start_time.tv_nsec;
		}
		//printf("elapsed: %d.%09ld\n", (int)elapsed.tv_sec, elapsed.tv_nsec); // debug
		//printf("elapsed: %lf\n", elapsed.tv_sec + (elapsed.tv_nsec / ONE_NANOSEC)); //debug
		//printf("elapsed ms: %lf\n", (elapsed.tv_sec + (elapsed.tv_nsec / ONE_NANOSEC)) * 1000.0f); // debug


		// Return time in milliseconds
		return ((size_t)((elapsed.tv_sec + (elapsed.tv_nsec / ONE_NANOSEC)) * 1000.0f));
	}

}