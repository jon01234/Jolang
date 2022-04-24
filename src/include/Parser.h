#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"
#include "Node.h"
#include "Token.h"
#include "Error.h"
#include <string.h>

typedef struct
{
	Lexer* lexer;
	Token* currentToken;
	Token* lastToken;
} Parser;

Parser* CreateParser(Lexer* lexer);
Node* Parser_Parse(Parser* parser);
Node* Parser_ParseStatements(Parser* parser);
Node* Parser_ParseStatement(Parser* parser);
Node* Parser_ParseFunction(Parser* parser);
Node* Parser_ParseFunctionBody(Parser* parser, Node* node);
Node* Parser_ParseFunctionCall(Parser* parser);
Node* Parser_ParseVariableDecl(Parser* parser);
void Parser_Next(Parser* parser);
void Parser_Consume(Parser* parser, int token);

#endif
