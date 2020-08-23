#ifndef CHECKSEM_H
#define CHECKSEM_H

#include <string>
#include <stack>
#include "node.h"

void checkSem(node_t *, int, int);
bool verify(string, stack<string>);
void insert(string);
string *getSymbolTable();
void symbol_table();

#endif
