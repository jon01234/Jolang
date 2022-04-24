#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdarg.h>

int Error(const char* err, ...);
int Warn(const char* err, ...);

#endif
