#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "UntitledEngine.h"
#include "UntitledEngineErrors.h"

namespace UntitledEngine {

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

	int init() {

		// Initialise GLFW
		if (!glfwInit()) {
			fatalError("Failed to initialize GLFW\n");
			return -1;
		}

		getGameTicks();

		glfwWindowHint(GLFW_SAMPLES, 4);                        // 4x antialiasing
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);          // We want OpenGL 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);    // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

		return 0;
	}

	size_t  getGameTicks(void) {

		static struct timespec _start_time = {-1,-1}; // time of init
		struct timespec current_time;       // time since init
		struct timespec elapsed;

		if (_start_time.tv_sec == -1) // init
			current_utc_time(&_start_time);
		printf("start: %d.%09ld\n", (int)_start_time.tv_sec, (long)_start_time.tv_nsec);

		//std::this_thread::sleep_for(std::chrono::milliseconds(1500));

		current_utc_time(&current_time);
		printf("current: %d.%09ld\n", (int)current_time.tv_sec, (long)current_time.tv_nsec);

		elapsed.tv_sec = current_time.tv_sec - _start_time.tv_sec;
		if(current_time.tv_nsec >= _start_time.tv_nsec)
			elapsed.tv_nsec = current_time.tv_nsec - _start_time.tv_nsec;
		else    {
			elapsed.tv_sec--;
			elapsed.tv_nsec = 1000000000 + current_time.tv_nsec - _start_time.tv_nsec;
		}
		printf("elapsed: %d.%09ld\n", (int)elapsed.tv_sec, (long)elapsed.tv_nsec);
		printf("elapsed: %lf\n", elapsed.tv_sec + elapsed.tv_nsec / 1000000000.0f);
		printf("elapsed ms: %lf\n", (elapsed.tv_sec + elapsed.tv_nsec / 1000000000.0f) * 1000);


		return ((size_t)((elapsed.tv_sec + elapsed.tv_nsec / 1000000000.0f) * 1000.0f));
	}

}