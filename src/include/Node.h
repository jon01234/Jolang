#ifndef NODE_H
#define NODE_H

#include <stddef.h>

typedef struct Node
{
	enum
	{
		//NODE_STATEMENT,
		NODE_PROGRAM,
		NODE_FUNCTION_DECL,
		NODE_FUNCTION_CALL,
		NODE_FUNCTION_ARG,
		NODE_STRING,
		NODE_VARIABLE_DECL,
		NODE_VARIABLE,
		NODE_NOOP
	} type;
	
	// Program
	struct Node** program;

	// Function decl
	char* function_name;
	struct Node** function_body;
	struct Node** function_args;
	size_t function_args_size;
	char* function_return_type;

	// Function call
	char* function_call_name;
	struct Node** function_call_args;
	size_t function_call_args_size;

	// Function arg
	char* arg_name;
	char* arg_type;

	// String
	char* string_value;

	// Variable decl
	char* variable_decl_name;
	char* variable_decl_type;
	char* variable_decl_value;

	// Variable 
	char* variable_name;
} Node;

Node* CreateNode(int type);

#endif
