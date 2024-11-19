#include "lp_interp.h"
#include "lp_debug.h"

char *
lp_strdup(const char *string)
{
	char *s;

	s = malloc(strlen(string) + 1);
	if (s == NULL) {
		lp_debug("lp_strdup() failed with malloc: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	strcpy(s, string);
	return s;
}
