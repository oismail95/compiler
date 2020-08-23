#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include "token.h"
using namespace std;

typedef struct node_t{
  string label;
  struct node_t *child1;
  struct node_t *child2;
  struct node_t *child3;
  struct node_t *child4;
  token tk_id;
  token tk_num;
  token tk_oper;
  token tk_delim;
}node_t;

node_t *getNode(string);

#endif
