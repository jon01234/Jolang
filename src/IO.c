#include "include/IO.h"
#include "include/Error.h"
#include <stdio.h>
#include <stdlib.h>

char* ReadFile(char* filename)
{
	FILE* f = fopen(filename, "rb"); // For some reason I need to use read byte instead of read

	if (f == NULL)
	{
		Error("Could not open file %s\n", filename);
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	char* string = malloc(fsize + 1);
	fread(string, fsize, 1, f);
	fclose(f);
	string[fsize] = '\0'; // Terminate string

	return string;
}
	
void WriteFile(char* filename, char* text)
{
	FILE* f = fopen(filename, "wb");

	if (f == NULL)
	{
		Error("Could not write file %s", filename);
	}

	fprintf(f, text);
	fclose(f);
}
		
