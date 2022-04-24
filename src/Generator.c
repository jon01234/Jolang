// TODO: write the rest of the funcitions 
// Functions work well with structure

#include "include/Generator.h"
#include <string.h>
#include <stdlib.h>
#include "include/Error.h"

char* gen_statement(Node* statement)
{
	switch (statement->type)
	{
	case NODE_FUNCTION_DECL: return gen_function_decl(statement); break;
	case NODE_FUNCTION_CALL: return gen_function_call(statement); break;
	case NODE_VARIABLE_DECL: return gen_variable_decl(statement); break;
	case NODE_VARIABLE: return gen_variable(statement); break;
	default:
		return NULL;
		break;
	}
}

char* gen_function_decl(Node* function)
{
	char* template = "global %s\n"
					 "%s:\n";
	
	char* asm_code = calloc(strlen(template) + (strlen(function->function_name) * 2) + 1, sizeof(char));
	
	sprintf(asm_code, template, function->function_name, function->function_name);

	size_t func_size = sizeof(function->function_body) / sizeof(function->function_body[0]);
	
	for (int i = 0; i < func_size; i++)
	{
		Node* currentNode = function->function_body[i];
		char* statement = gen_statement(currentNode);
		asm_code = realloc(asm_code, strlen(asm_code) + strlen(statement) + 1);
		strcat(asm_code, statement);
	}	
	return asm_code;
}

char* gen_function_call(Node* function)
{
	return "";
}

char* gen_variable_decl(Node* variable)
{
	char* asm_code = calloc(1, sizeof(char));
	if (strcmp(variable->variable_decl_name, "return") == 0)
	{		char* template = "mov eax, ";

		asm_code = realloc(
			asm_code, strlen(template) + strlen(variable->variable_decl_name) + strlen("\nret") + 1
		);
		sprintf(asm_code, "%s%s\nret", template, variable->variable_decl_value);

		return asm_code;
	}
}

char* gen_variable(Node* variable)
{
	return "";
}
