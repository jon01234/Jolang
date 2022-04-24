#ifndef JOLANG_H
#define JOLANG_H

#include "Lexer.h"
#include "Parser.h"
#include "Node.h"

void Jolang_Compile(char* code);
void Jolang_CompileFile(char* filename);

#endif
