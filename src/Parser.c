#include "include/Parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Parser* CreateParser(Lexer* lexer)
{
	Parser* parser = calloc(1, sizeof(Parser));
	parser->lexer = lexer;
	parser->currentToken = Lexer_GetNextToken(parser->lexer);
	parser->lastToken = parser->currentToken;

	return parser;
}

Node* Parser_Parse(Parser* parser)
{
	return Parser_ParseStatements(parser);
}

Node* Parser_ParseStatements(Parser* parser)
{
	Node* node = CreateNode(NODE_PROGRAM);

	node->program = calloc(0, sizeof(struct Node*));

	for (int i = 0; parser->currentToken->type != TOKEN_EOF; i++)
	{
		switch (parser->currentToken->type)
		{
		case TOKEN_ID: // A statement cannot start with anything but Identifier
				node->program = realloc(node->program, i + 1);
				node->program[i] = Parser_ParseStatement(parser);
		default: break;
		}
		node->program_size += 1;
	}

	return node;
}

Node* Parser_ParseStatement(Parser* parser)
{
	if (strcmp(parser->currentToken->value, "function") == 0)
		return Parser_ParseFunction(parser);

	Parser_Next(parser);

	if (parser->currentToken->type == TOKEN_LPAREN)
		return Parser_ParseFunctionCall(parser);	
	if (parser->currentToken->type == TOKEN_ARROW)
			return Parser_ParseVariableDecl(parser);

	Parser_Consume(parser, TOKEN_LPAREN);
	return CreateNode(NODE_NOOP);
}

Node* Parser_ParseFunction(Parser* parser)
{
	Node* node = CreateNode(NODE_FUNCTION_DECL);

	Parser_Consume(parser, TOKEN_ID); // Function decl (function)
	Parser_Consume(parser, TOKEN_ID); // Function name
	node->function_name = parser->lastToken->value;
	Parser_Consume(parser, TOKEN_LPAREN);	

	if (parser->currentToken->type == TOKEN_RPAREN) // We have no args
	{
		Parser_Consume(parser, TOKEN_RPAREN);
		Parser_Consume(parser, TOKEN_ARROW);
		Parser_Consume(parser, TOKEN_ID);
		node->function_return_type = parser->lastToken->value;
		return Parser_ParseFunctionBody(parser, node);	
	}
	// Parse the arguments
	node->function_args = calloc(1, sizeof(struct Node*));
	node->function_args[0] = CreateNode(NODE_FUNCTION_ARG);
	// First arg
	node->function_args[0]->arg_name = parser->currentToken->value;
	Parser_Consume(parser, TOKEN_ID); // Args in function decl may only consist of identifiers
	Parser_Consume(parser, TOKEN_ARROW);
	Parser_Consume(parser, TOKEN_ID); // Arg type

	if (strcmp(parser->lastToken->value, "Array") == 0)
	{
		Parser_Consume(parser, TOKEN_LBRACKET); // [
		Parser_Consume(parser, TOKEN_ID); // TYPE
		node->function_args[0]->arg_type = malloc(strlen(parser->lastToken->value) + strlen("-arr") + 1);
		node->function_args[0]->arg_type = strcat(parser->lastToken->value, "-arr");
		Parser_Consume(parser, TOKEN_RBRACKET); // ]
	} 
	else // its not array
	{
		node->function_args[0]->arg_type = parser->currentToken->value;
	}

	node->function_args_size = 1;
	
	// If we only have one arg
	if (parser->currentToken->type == TOKEN_RPAREN)
		return Parser_ParseFunctionBody(parser, node);

	if (parser->currentToken->type != TOKEN_COMMA)
	{
		Error("Expected ',' or ')' but got '%s' in line %d", parser->currentToken->value, parser->lexer->lineno);
		exit(EXIT_FAILURE);
	}

	while (parser->currentToken->type == TOKEN_COMMA)
	{
		node->function_args_size += 1;

		Parser_Next(parser); // Skip comma
		Parser_Consume(parser, TOKEN_ID);

		node->function_args = realloc(node->function_args, node->function_args_size * sizeof(struct Node*));
		node->function_args[node->function_args_size - 1] = CreateNode(NODE_FUNCTION_ARG);
		
		node->function_args[node->function_args_size - 1]->arg_name = parser->lastToken->value;
		Parser_Consume(parser, TOKEN_ARROW); // ->
		
		if (strcmp(parser->currentToken->value, "Array") == 0)
		{
			Parser_Next(parser);
			Parser_Consume(parser, TOKEN_LBRACKET); // [
			Parser_Consume(parser, TOKEN_ID); // TYPE
			node->function_args[node->function_args_size - 1]->arg_type = malloc(strlen(parser->lastToken->value) + strlen("-arr") + 1);
			node->function_args[node->function_args_size - 1]->arg_type = strcat(parser->lastToken->value, "-arr");
			Parser_Consume(parser, TOKEN_RBRACKET); // ]
		} 
		else 
		{
			node->function_args[node->function_args_size - 1]->arg_type = parser->currentToken->value;
			Parser_Next(parser); // Now currentToken is either , or ) (ideally)
		}	
	}
	
	Parser_Consume(parser, TOKEN_RPAREN); // Closing parentethis

	Parser_Consume(parser, TOKEN_ARROW);
	node->function_return_type = parser->currentToken->value;
	Parser_Consume(parser, TOKEN_ID);

	return Parser_ParseFunctionBody(parser, node);
}

Node* Parser_ParseFunctionBody(Parser* parser, Node* node)
{	
	Parser_Consume(parser, TOKEN_LBRACE); // Begining of body
	int i = 0;
	node->function_body = calloc(i, sizeof(struct Node*));
	while (parser->currentToken->type != TOKEN_RBRACE) // Not end of body
	{
		node->function_body = realloc(node->function_body, i + 1);
		node->function_body[i] = Parser_ParseStatement(parser);
		i += 1;
		node->function_body_size = i;
	}
	Parser_Consume(parser, TOKEN_RBRACE); // End of function

	return node;
}

Node* Parser_ParseFunctionCall(Parser* parser)
{
	Node* node = CreateNode(NODE_FUNCTION_CALL);
	node->function_call_name = parser->lastToken->value;

	Parser_Next(parser);

	if (parser->currentToken->type == TOKEN_RPAREN)
	{
		Parser_Consume(parser, TOKEN_RPAREN);
		Parser_Consume(parser, TOKEN_SEMI);
		return node;
	}

	// Same concept as function decl args
	node->function_call_args = calloc(1, sizeof(struct Node*));
	node->function_call_args[0] = CreateNode(NODE_FUNCTION_ARG);

	node->function_call_args[0]->arg_name = parser->currentToken->value;

	switch (parser->currentToken->type)
	{
	case TOKEN_STRING:
		node->function_call_args[0]->arg_type = "string";
		break;
	case TOKEN_ID:
		node->function_call_args[0]->arg_type = "variable";
		break;
	case TOKEN_INT:
		node->function_call_args[0]->arg_type = "int";
		break;
	default:
		Error(
			"Expected an Identifier, string or int but got type '%s' at line: %d", 
			typeasstring(parser->currentToken->type), 
			parser->lexer->lineno
		);
		exit(EXIT_FAILURE);
	}

	node->function_call_args_size = 1;

	Parser_Next(parser);

	if (parser->currentToken->type == TOKEN_RPAREN)
	{
		
		Parser_Next(parser);
		Parser_Consume(parser, TOKEN_SEMI);
		return node;
	}

	if (parser->currentToken->type != TOKEN_COMMA)
	{
		Error("Expected ',' or ')' but got '%s' at line %d", parser->currentToken->value, parser->lexer->lineno);
		exit(EXIT_FAILURE);
	}

	while (parser->currentToken->type == TOKEN_COMMA)
	{
		Parser_Next(parser);

		node->function_call_args_size += 1;
		int i = node->function_call_args_size - 1;

		node->function_call_args = realloc(node->function_call_args, node->function_call_args_size * sizeof(struct Node*));
		node->function_call_args[i] = CreateNode(NODE_FUNCTION_ARG);
	
		node->function_call_args[i]->arg_name = parser->currentToken->value;
	
		switch (parser->currentToken->type)
		{
		case TOKEN_STRING:
			node->function_call_args[i]->arg_type = "string";
			break;
		case TOKEN_ID:
			node->function_call_args[i]->arg_type = "variable";
			break;
		case TOKEN_INT:
			node->function_call_args[i]->arg_type = "int";
			break;
		default:
			Error(
				"Expected an Identifier, string or int but got type '%s' at line: %d", 
				typeasstring(parser->currentToken->type), 
				parser->lexer->lineno
			);
			exit(EXIT_FAILURE);
		}
	
		Parser_Next(parser);	
	}

	Parser_Consume(parser, TOKEN_RPAREN);
	Parser_Consume(parser, TOKEN_SEMI);

	return node;
}

Node* Parser_ParseVariableDecl(Parser* parser)
{
	Node* node = CreateNode(NODE_VARIABLE_DECL);

	node->variable_decl_name = parser->lastToken->value; // NAME
	Parser_Consume(parser, TOKEN_ARROW); // ->
	Parser_Consume(parser, TOKEN_ID); // TYPE
	node->variable_decl_type = parser->lastToken->value;

	if (strcmp(parser->lastToken->value, "Array") == 0)
	{
		Parser_Consume(parser, TOKEN_LBRACKET); // [
		Parser_Consume(parser, TOKEN_ID); // TYPE
		node->function_args[0]->arg_type = strcat(strcat("Array[", parser->lastToken->value), "]");
		Parser_Consume(parser, TOKEN_RBRACKET); // ]
	}
	Parser_Consume(parser, TOKEN_EQUALS); // =
	
	if (parser->currentToken->type != TOKEN_STRING && parser->currentToken->type != TOKEN_INT)
	{
		Error("Expected a string or int but got '%s' on line %d", parser->currentToken->value, parser->lexer->lineno);
		exit(EXIT_FAILURE);
	}

	node->variable_decl_value = parser->currentToken->value;
	
	Parser_Next(parser); // skip var value
	Parser_Consume(parser, TOKEN_SEMI);

	return node;
}

void Parser_Next(Parser* parser)
{
	parser->lastToken= parser->currentToken;
	parser->currentToken = Lexer_GetNextToken(parser->lexer);
}

void Parser_Consume(Parser* parser, int token)
{
	if (parser->currentToken->type == token)
	{
		Parser_Next(parser);
	}
	else
	{
		Error(
			"Unexpected token '%s' at line %d. Expected token of type %s", 
			parser->currentToken->value, 
			parser->lexer->lineno,
			typeasstring(token)
		);
	}
}

