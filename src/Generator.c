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

	for (int i = 0; i < function->function_body_size; i++)
	{
		Node* currentNode = function->function_body[i];
		char* statement = gen_statement(currentNode);
		asm_code = realloc(asm_code, strlen(asm_code) + strlen(statement) + 1);
		strcat(asm_code, statement);

		if (i == function->function_body_size - 1 && currentNode->type == NODE_VARIABLE_DECL && strcmp(currentNode->variable_decl_name, "return") == 0)
		{
			char* template = "mov eax, ";

			char* code = calloc(
				1, strlen(template) + strlen(currentNode->variable_decl_name) + strlen("\nret") + 1
			);
			sprintf(code, "%s%s\nret\n\n", template, currentNode->variable_decl_value);

			asm_code = realloc(asm_code, strlen(asm_code) + strlen(code) + 1);
			strcat(asm_code, code);
		}
		else
		{
			Error("All functions must contain a return value at the end");
		}
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
}

char* gen_variable(Node* variable)
{
	return "";
}
