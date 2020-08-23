#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
using namespace std;

enum tokenID{IDENT_tk, NUM_tk, KWD_tk, OPER_tk, DELIM_tk, EOF_tk};

typedef enum{
  START, INASSIGN, INCOMMENT, INNUM, INID, DONE
}state_t;

typedef struct token{
  tokenID token;
  string str;
  int line_num;
}token;

#endif
