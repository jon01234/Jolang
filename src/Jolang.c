#include "include/Jolang.h"
#include "include/Generator.h"
#include "include/Error.h"
#include "include/IO.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void Jolang_Compile(char* code)
{
	Lexer* lexer = CreateLexer(code);
	Parser* parser = CreateParser(lexer);

	Node* program = Parser_Parse(parser);
	size_t programSize = sizeof(program->program) / sizeof(program->program[0]);

	char* template = "section .text:\n"
					 "global _start\n"
					 "_start:\n"
					 "call main\n"
					 "mov ebx, eax\n" // Putting return value of function main in arg for exit code
					 "mov eax, 0x1\n"
					 "int 0x80\n\n";

	char* ASMProgram = calloc(1, sizeof(char));

	ASMProgram = realloc(ASMProgram, strlen(template) + 1);
	strcat(ASMProgram, template);

	for (int i = 0; i < programSize; i++)
	{
		char* str = gen_statement(program->program[i]);
		ASMProgram = realloc(ASMProgram,	strlen(ASMProgram) + strlen(str) + 1);
		strcat(ASMProgram, str);
	}

	WriteFile("out.asm", ASMProgram);
}
void Jolang_CompileFile(char* filename)
{
	char* code = ReadFile(filename);
	Jolang_Compile(code);
}
