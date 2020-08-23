#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <stack>
#include "checkSem.h"
#include "node.h"
using namespace std;

static stack<string> tk_id;

//Got that code snippet from https://www.geeksforgeeks.org/tree-traversals-inorder-preorder-and-postorder/
void checkSem(node_t *node, int level, int count){
  bool check;

  if(node == NULL)
    return; 

  //If the nonterminal is 'V' and an id is detected, it'll verify and check if it is in the
  //symbol table.
  //Else will check if an id is used below the definition and if it does not define, it'll
  //give an error and terminate.
  if(node->label == "V" && node->tk_id.str != ""){
    check = verify(node->tk_id.str, tk_id);

    if(!check){
      insert(node->tk_id.str);
    }
    else{
      cerr << "Fatal: A variable is redefined on line " << node->tk_id.line_num << endl;
      cerr << "Usage: " << node->tk_id.str << endl;
      exit(0);
    }
  }
  else{
    if(node->tk_id.str != ""){
      check = verify(node->tk_id.str, tk_id);

      if(!check){
        cerr << "Fatal: An undefined variable exists on line " << node->tk_id.line_num << endl;
        cerr << "Usage: " << node->tk_id.str << endl;
        exit(0);
      }
    }
  }

  checkSem(node->child1, level + 1, count + 1);
  checkSem(node->child2, level + 1, count + 1);
  checkSem(node->child3, level + 1, count + 1);
  checkSem(node->child4, level + 1, count + 1);
}

//Got that code snippet from https://www.geeksforgeeks.org/stack-in-cpp-stl/
bool verify(string id_str, stack<string> tk_id){
  while(!tk_id.empty()){
    if(id_str == tk_id.top())
      return true;
    tk_id.pop();
  }

  return false;
}

void insert(string id){
  tk_id.push(id);
}

//Got that code snippet from https://www.geeksforgeeks.org/return-local-array-c-function/
string *getSymbolTable(){
  int size = tk_id.size();
  string *r = new string[size];

  for(int i = 0; i < size; i++){
    r[i] = tk_id.top();
    tk_id.pop();
  }

  return r;
}

//Got that code snippet from https://www.geeksforgeeks.org/stack-in-cpp-stl/
void symbol_table(){
  cout << endl << "Symbol Table" << endl;
  cout << "-------------------------------------------------" << endl;

  while(!tk_id.empty()){
    cout << tk_id.top() << endl;
    tk_id.pop();
  }
  cout << endl;
}
