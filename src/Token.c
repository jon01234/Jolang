#include "include/Token.h"
#include <stdlib.h>

char* types[] = 
{
	"TOKEN_ID",
	"TOKEN_STRING",
	"TOKEN_LPAREN",
	"TOKEN_RPAREN",
	"TOKEN_ARROW",
	"TOKEN_EQUALS",
	"TOKEN_COMMA",
	"TOKEN_LBRACKET",
	"TOKEN_RBRACKET",
	"TOKEN_GREATER",
	"TOKEN_SMALLER",
	"TOKEN_INT",
	"TOKEN_LBRACE",
	"TOKEN_RBRACE",
	"TOKEN_SEMI",
	"TOKEN_UNKNOWN",
	"TOKEN_EOF"
};

char* typeasstring(int type)
{
	return types[type];
}

Token* CreateToken(char* value, int type)
{
	Token* token = calloc(1, sizeof(Token));

	token->value = value;
	token->type = type;

	return token;
}
