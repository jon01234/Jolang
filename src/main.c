#include "include/Jolang.h"
#include "include/Error.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("\033[1;13mUsage:\033[0m Jolang <file>\n");
		return 1;
	}
	Jolang_CompileFile(argv[1]);

	return 0;
}
