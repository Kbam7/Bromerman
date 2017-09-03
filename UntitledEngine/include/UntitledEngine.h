#include <ctime>
#include <sys/time.h>

#ifdef __MACH__
# include <mach/clock.h>
# include <mach/mach.h>
#endif

#include <chrono>   // For delay
#include <thread>   // For delay

namespace UntitledEngine {

    extern int init();
	extern size_t getGameTicks(void);          // returns current time in ms
	extern void current_utc_time(struct timespec *ts);

}