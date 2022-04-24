#include "include/Lexer.h"
#include "include/Error.h"
#include <string.h>
#include <stdlib.h>

Lexer* CreateLexer(char* code)
{
	Lexer* lexer = calloc(1, sizeof(Lexer));
	
	lexer->code = code;
	lexer->code_size = strlen(code);
	lexer->i = 0;
	lexer->c = lexer->code[lexer->i];
	lexer->lineno = 1;

	return lexer;
}

void Lexer_Advance(Lexer* lexer)
{
	if (!(lexer->i < lexer->code_size && lexer->c != '\0')) return;

	lexer->i += 1;
	lexer->c = lexer->code[lexer->i];
}

void Lexer_SkipsWhitespace(Lexer* lexer)
{
	while (lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n' || lexer->c == '\r')
	{
		if (lexer->c == '\n')
			lexer->lineno += 1;

		Lexer_Advance(lexer);
	}
}

char Lexer_Peak(Lexer* lexer, int n)
{
	return lexer->i + n < lexer->code_size ? lexer->code[lexer->i + n] : lexer->code[lexer->code_size];
}

char Lexer_PeakNext(Lexer* lexer)
{
	return Lexer_Peak(lexer, 1);
}

Token* Lexer_GetIdentifier(Lexer* lexer)
{
	char* value = calloc(1, sizeof(char));
	
	while (isalnum(lexer->c) || lexer->c == '_') // Because of is alpha in the if statement then we have [a-zA-Z_][a-zA-Z_0-9]* in regex
	{
		value = realloc(value, (strlen(value) + 2) * sizeof(char));
		strcat(value, (char[]){ lexer->c, 0 });
		Lexer_Advance(lexer);
	}

	return CreateToken(value, TOKEN_ID);
}

Token* Lexer_GetString(Lexer* lexer)
{
	char* value = calloc(1, sizeof(char));

	Lexer_Advance(lexer); // Skip first Qoute

	while(lexer->c != '"')
	{
		value = realloc(value, (strlen(value) + 2) * sizeof(char));
		strcat(value, (char[]){ lexer->c, 0 });
		Lexer_Advance(lexer);
	}

	Lexer_Advance(lexer);

	return CreateToken(value, TOKEN_STRING);
}

Token* Lexer_GetNumber(Lexer* lexer)
{
	// same idea as identifier
	char* value = calloc(1, sizeof(char));
	while (isdigit(lexer->c))
	{
		value = realloc(value, (strlen(value) + 2) * sizeof(char));
		strcat(value, (char[]){ lexer->c, 0 });
		Lexer_Advance(lexer);
	}

	return CreateToken(value, TOKEN_INT);
}

Token* Lexer_GetNextToken(Lexer* lexer)
{
	while (lexer->c != '\0')
	{
		Lexer_SkipsWhitespace(lexer);

		if (isalpha(lexer->c) || lexer->c == '_') // In regex it would be [a-zA-Z_]
			return Lexer_GetIdentifier(lexer);

		if (isdigit(lexer->c)) // Regex [0-9] or \d
			return Lexer_GetNumber(lexer);

		if (lexer->c == '"')
			return Lexer_GetString(lexer);

		if (lexer->c == '-' && Lexer_PeakNext(lexer) == '>')
		{
			Lexer_Advance(lexer);
			Lexer_Advance(lexer);
			return CreateToken("->", TOKEN_ARROW);
		}

		switch (lexer->c)
		{
		case '(': Lexer_Advance(lexer); return CreateToken("(", TOKEN_LPAREN); break;
		case ')': Lexer_Advance(lexer); return CreateToken(")", TOKEN_RPAREN); break;
		case ',': Lexer_Advance(lexer); return CreateToken(",", TOKEN_COMMA); break;
		case '=': Lexer_Advance(lexer); return CreateToken("=", TOKEN_EQUALS); break;
		case '[': Lexer_Advance(lexer); return CreateToken("[", TOKEN_LBRACKET); break;
		case ']': Lexer_Advance(lexer); return CreateToken("]", TOKEN_RBRACKET); break;
		case '{': Lexer_Advance(lexer); return CreateToken("{", TOKEN_LBRACE); break;
		case '}': Lexer_Advance(lexer); return CreateToken("}", TOKEN_RBRACE); break;
		case ';': Lexer_Advance(lexer); return CreateToken(";", TOKEN_SEMI); break;
		case '\0': break;
		default: 
			Error("Unknown character '%c' with ASCII value '%d' at line: %d\n", lexer->c, lexer->c, lexer->lineno); 
			Lexer_Advance(lexer); 
			return CreateToken("\"UNKNOWN TOKEN\"", TOKEN_UNKNOWN); 
			break;
		}
	}

	return CreateToken(NULL, TOKEN_EOF);
}
