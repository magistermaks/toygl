
#include <sys/time.h>
#include <unistd.h>

typedef unsigned long long long_time_t;

long_time_t get_time() {
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return tp.tv_sec * 1000ull * 1000ull + tp.tv_usec;
}

bool framerate( int target, long_time_t& fps, long_time_t& frame, long_time_t& wait ) {

	const long_time_t now = get_time();
	const long_time_t max = 1000000 / target;

	static int count = 0;
	static long_time_t start = now;
	static long_time_t begin = now;

	fps = ++count;
	frame = (now - begin);
	wait = max - frame;

	if( now >= begin && max >= frame ) {

		usleep( wait );

	}else{

		wait = 0;

	}

	begin = get_time();

	if( now - start >= 1000000ull ) {
	    start = now;
	    count = 0;
	    return true;
	}

	return false;

}
