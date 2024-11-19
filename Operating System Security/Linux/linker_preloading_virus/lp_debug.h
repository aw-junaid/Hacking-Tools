#include <stdarg.h>
#include <stdio.h>

#if defined DEBUG
	#define lp_debug(...) {\
	do {\
		fprintf(stderr, "[%s:%s:%d] ", __FILE__, __func__, __LINE__); \
		fprintf(stderr, __VA_ARGS__);	\
	} while(0); \
}
#else
	#define lp_debug(...)
#endif
