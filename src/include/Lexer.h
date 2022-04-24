#ifndef LEXER_H
#define LEXER_H

#include "Token.h"
#include <stddef.h>

typedef struct 
{
	char* code;
	size_t code_size;
	char c;
	unsigned int i;
	unsigned int lineno;
} Lexer;

Lexer* CreateLexer(char* code);
void Lexer_Advance(Lexer* lexer);
void Lexer_SkipsWhitespace(Lexer* lexer);
char Lexer_Peak(Lexer* lexer, int n);
char Lexer_PeakNext(Lexer* lexer);
Token* Lexer_GetIdentifier(Lexer* lexer);
Token* Lexer_GetString(Lexer* lexer);
Token* Lexer_GetNumber(Lexer* lexer);
Token* Lexer_GetNextToken(Lexer* lexer);

#endif

