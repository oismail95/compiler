#ifndef TESTSCANNER_H
#define TESTSCANNER_H

#include <fstream>
#include <string>
#include "node.h"

using namespace std;
node_t *parser(ifstream &, const char *);
node_t *S();
node_t *V();
node_t *B();
node_t *Q();
node_t *T();
node_t *A();
node_t *I();
node_t *M();
node_t *H();
node_t *R();
node_t *Z();
node_t *W();
node_t *G();
node_t *E();
string getString(char);
string check_err_and_append(int, char, string);

#endif
