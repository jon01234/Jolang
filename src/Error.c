#include "include/Error.h"
#include <string.h>

int Error(const char* err, ...)
{
	va_list args;
	int done;	
	
	printf("\033[1;91mERROR: \033[0m");

	va_start(args, err);
	done = vfprintf(stdout, err, args);
	va_end(args);

	printf("\n");

	return done;
}

int Warn(const char* err, ...)
{
	va_list args;
	int done;

	printf("\033[1;93mWARNING: \033[0m");

	va_start(args, err);
	done = vfprintf(stdout, err, args);
	va_end(args);

	printf("\n");

	return done;
}
