#ifndef TOKEN_H
#define TOKEN_H

typedef struct
{
	char* value;
	enum
	{
		TOKEN_ID,
		TOKEN_STRING,
		TOKEN_LPAREN,
		TOKEN_RPAREN,
		TOKEN_ARROW,
		TOKEN_EQUALS,
		TOKEN_COMMA,
		TOKEN_LBRACKET,
		TOKEN_RBRACKET,
		TOKEN_INT,
		TOKEN_LBRACE,
		TOKEN_RBRACE,
		TOKEN_SEMI,
		TOKEN_UNKNOWN,
		TOKEN_EOF
	} type;
} Token;	

char* typeasstring(int type);

Token* CreateToken(char* value, int type);

#endif
