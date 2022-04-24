#ifndef GENERATOR_H
#define GENERATOR_H

#include "Node.h"

char* gen_statement(Node* statement);
char* gen_function_decl(Node* function);
char* gen_function_call(Node* function);
char* gen_variable_decl(Node* variable);
char* gen_variable(Node* variable);

#endif
