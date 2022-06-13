#include "include/Jolang.h"
#include "include/Error.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		Error("Usage Jolang <file>");
		return 1;
	}
	Jolang_CompileFile(argv[1]);

	return 0;
}
