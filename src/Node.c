#include "include/Node.h"
#include <stdlib.h>

Node* CreateNode(int type)
{
	Node* node = calloc(1, sizeof(struct Node));	
	node->type = type;

	node->program = NULL;
	node->program_size = 0;

	node->function_name = NULL;
	node->function_body = NULL;
	node->function_body_size = 0;
	node->function_args = NULL;
	node->function_args_size = 0;
	node->function_return_type = NULL;

	node->function_call_name = NULL;
	node->function_call_args = NULL;
	node->function_call_args_size = 0;

	node->arg_name = NULL; // Could be the value of arg in function call
	node->arg_type = NULL;

	node->string_value = NULL;

	node->variable_decl_name = NULL;
	node->variable_decl_value = NULL;

	node->variable_name = NULL;

	return node;
}
