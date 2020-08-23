#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <string>
#include "parser.h"
#include "scanner.h"
#include "token.h"
#include "node.h"

//Declared as a global variable to pass it to implement through the parse tree
static token tkn;

using namespace std;
node_t *parser(ifstream &file, const char *fName){

  int n;
  char *char_arr;
  int pos;
  int curr_state;
  int next_state;
  string line_str;
  node_t *root;

  file.open(fName);

  tkn.line_num = 1;
  curr_state = 0;

  //Got that code from https://www.youtube.com/watch?v=qI-tUsujrCU
  while(!file.eof()){

    getline(file, line_str);

    if(!file.eof()){

      n = line_str.length();
      char_arr = new char[n + 1];

      strcpy(char_arr, line_str.c_str());

      pos = 0;

      //Outputs the character and the designated line number
      while(char_arr[pos] != '\0'){

        //Calling an integer to grab the column number from scanner file
        //and checking for error and appending the token in a character
        next_state = fad_driver(char_arr[pos], curr_state);
        tkn.str = check_err_and_append(next_state, char_arr[pos], tkn.str);

        //Checks whether the token is an identifier, numeric, delimeter, or an operator
        if(next_state == 1001 || next_state == 1002 || next_state == 1003 || next_state == 1004){
          call_token(tkn.str, tkn.line_num, next_state);
          tkn.str = "";
          next_state = 0;

          if(!isspace(char_arr[pos])){
            curr_state = 0;
            next_state = fad_driver(char_arr[pos], curr_state);
            tkn.str = check_err_and_append(next_state, char_arr[pos], tkn.str);
          }
        }

        //Shifts to the next state in the table
        curr_state = next_state;
        pos++;
      }

      //When the end of the line gets detected, the line number gets incremented
      if(char_arr[pos] == '\0'){
        next_state = fad_driver(char_arr[pos], curr_state);

        call_token(tkn.str, tkn.line_num, next_state);
        tkn.str = "";
        next_state = 0;

        curr_state = next_state;
        tkn.line_num++;
      }
    }
    else{//Once the EOF occurs, the EOF token will be outputted
      //Decrements the line number back to set the current line position
      tkn.line_num--;

      call_token("", tkn.line_num, 1005);
    }
  }

  file.close();



  //Here is where the parsing progress begins.
  tkn = scanner();

  root = S();

  //If the token reaches EOF, it'll return back to main and then output in preorder.
  //Else will give an error and terminate.
  if(tkn.token == EOF_tk){
    cout << "End of File Reached" << endl;
    return root;
  }
  else{
    cerr << "Fatal: EOF not reached on line " << tkn.line_num << endl;
    exit(0);
  }
}

//Here is where the node gets set up and created.
node_t *getNode(string label){
  node_t *p;

  p = new node_t;
  p->label = label;

  p->child1 = NULL;
  p->child2 = NULL;
  p->child3 = NULL;
  p->child4 = NULL;

  return p;
}

node_t *S(){
  //Establishes a new node.
  node_t *p = getNode("S");

  if(tkn.str != "program"){
    cerr << "Fatal: Keyword 'program' not defined on line " << tkn.line_num << endl;
    cerr << "Usage: " << tkn.str << endl;
    exit(0);
  }
  else{
    tkn = scanner();
    p->child1 = V();
    p->child2 = B();
    return p;
  }
}

node_t *V(){
  if(tkn.str == "var"){
    //Establishes a new node.
    node_t *p = getNode("V");
    tkn = scanner();

    if(tkn.token == IDENT_tk){
      p->tk_id = tkn;
      tkn = scanner();

      if(tkn.str == "."){
        tkn = scanner();
        p->child1 = V();
        return p;
      }
      else{
        cerr << "Fatal: '.' does not exist after the identifier on line " << tkn.line_num << endl;
        cerr << "Usage: " << tkn.str << endl;
        exit(0);
      }
    }
    else{
      cerr << "Fatal: Indentifier does not exist after the keyword, 'var' on line " << tkn.line_num << endl;
      cerr << "Usage: " << tkn.str << endl;
      exit(0);
    }
  }
  else{//Assume empty string.
    return NULL;
  }
}

node_t *B(){
  node_t *p = getNode("B");

  //If the token is not 'begin', it'll give an error.
  if(tkn.str != "begin"){
    cerr << "Fatal: Keyword 'begin' not defined on line " << tkn.line_num << endl;
    cerr << "Usage: " << tkn.str << endl;
    exit(0);
  }
  else{
    tkn = scanner();
    p->child1 = V();
    p->child2 = Q();

    //If the token is not 'end', it'll give an error.
    if(tkn.str != "end"){
      cerr << "Fatal: Keyword 'end' not defined on line " << tkn.line_num << endl;
      cerr << "Usage: " << tkn.str << endl;
      exit(0);
    }
    else{
      tkn = scanner();
      return p;
    }
  }
}

node_t *Q(){
  //If any of these tokens matched below, it'll perform the operation of nonterminals.
  //Else will return as empty.
  if(tkn.str == "scan" || tkn.str == "write" || tkn.str == "begin" || tkn.str == "if" || tkn.str == "repeat" || tkn.str == "let"){
    node_t *p = getNode("Q");
    p->child1 = T();

    if(tkn.str == "#"){
      tkn = scanner();
      p->child2 = Q();
      return p;
    }
    else{
      cerr << "Fatal: '#' not defined on line " << tkn.line_num << endl;
      cerr << "Usage: " << tkn.str << endl;
      exit(0);
    }
  }
  else{//Assume empty string.
    return NULL;
  }
}

node_t *T(){
  node_t *p = getNode("T");

  //If the token is 'scan', it'll jump to nonterminal, 'A'.
  if(tkn.str == "scan"){
    p->child1 = A();

    if(tkn.str == ","){
      tkn = scanner();
      return p;
    }
    else{
      cerr << "Fatal: ',' is not defined from scan on line " << tkn.line_num << endl;
      cerr << "Usage: " << tkn.str << endl;
      exit(0);
    }
  }//If the token is 'begin', it'll jump to nonterminal, 'B'.
  else if(tkn.str == "begin"){
    p->child1 = B();
    return p;
  }
  else if(tkn.str == "if"){//If the token is 'if', it'll jump to nonterminal 'I'.
    p->child1 = I();

    if(tkn.str == ","){
      tkn = scanner();
      return p;
    }
    else{
      cerr << "Fatal: ',' is not defined from if on line " << tkn.line_num << endl;
      cerr << "Usage: " << tkn.str << endl;
      exit(0);
    }
  }
  else if(tkn.str == "write"){//If the token is 'write', it'll jump to nonterminal 'W'.
    p->child1 = W();

    if(tkn.str == ","){
      tkn = scanner();
      return p;
    }
    else{
      cerr << "Fatal: ',' is not defined from write on line " << tkn.line_num << endl;
      cerr << "Usage: " << tkn.str << endl;
      exit(0);
    }
  }
  else if(tkn.str == "repeat"){//If the token is 'repeat', it'll jump to nonterminal 'G'.
    p->child1 = G();

    if(tkn.str == ","){
      tkn = scanner();
      return p;
    }
    else{
      cerr << "Fatal: ',' is not defined from repeat on line " << tkn.line_num << endl;
      cerr << "Usage: " << tkn.str << endl;
      exit(0);
    }
  }
  else if(tkn.str == "let"){//If the token is 'let', it'll jump to nonterminal 'E'.
    p->child1 = E();

    if(tkn.str == ","){
      tkn = scanner();
      return p;
    }
    else{
      cerr << "Fatal: ',' is not defined from let on line " << tkn.line_num << endl;
      cerr << "Usage: " << tkn.str << endl;
      exit(0);
    }
  }
  else{
    cerr << "Fatal: None of keywords matched on line " << tkn.line_num << endl;
    cerr << "Usage: " << tkn.str << endl;
    exit(0);
  }
}

node_t *G(){
  node_t *p = getNode("G");

  //If the string token is 'repeat', it'll jump to the nonterminals
  //and perform the operaiton.
  if(tkn.str == "repeat"){
    tkn = scanner();
    if(tkn.str == "["){
      tkn = scanner();
      p->child1 = M();
      p->child2 = Z();
      p->child3 = M();

      if(tkn.str == "]"){
        tkn = scanner();
        p->child4 = T();
        return p;
      }
      else{
        cerr << "Fatal: ']' is missing in the repeat statement on line " << tkn.line_num << endl;
        cerr << "Usage: " << tkn.str << endl;
        exit(0);
      }
    }
    else{
      cerr << "Fatal: '[' is missing in the repeat statement on line " << tkn.line_num << endl;
      cerr << "Usage: " << tkn.str << endl;
      exit(0);
    }
  }
  else{//Displays an error and terminates.
    cerr << "Fatal: 'repeat' is missing on line " << tkn.line_num << endl;
    cerr << "Usage: " << tkn.str << endl;
    exit(0);
  }
}

//This function is checking for the syntax of the let statement.
node_t *E(){
  node_t *p = getNode("E");

  if(tkn.str == "let"){
    tkn = scanner();
    if(tkn.token == IDENT_tk){
      p->tk_id = tkn;
      tkn = scanner();

      if(tkn.str == ":"){
        tkn = scanner();
        p->child1 = M();
        return p;
      }
      else{
        cerr << "Fatal: ':' is not defined in the let statement on line " << tkn.line_num << endl;
        cerr << "Usage: " << tkn.str << endl;
        exit(0);
      }
    }
    else{
      cerr << "Fatal: Identifier is not defined in the let statement on line " << tkn.line_num << endl;
      cerr << "Usage: " << tkn.str << endl;
      exit(0);
    }
  }
  else{
    cerr << "Fatal: 'let' is not defined on line " << tkn.line_num << endl;
    cerr << "Usage: " << tkn.str << endl;
    exit(0);
  }
}

node_t *W(){
  node_t *p = getNode("W");

  if(tkn.str == "write"){
    tkn = scanner();
    p->child1 = M();
    return p;
  }
  else{
    cerr << "Fatal: 'write' is not defined on line " << tkn.line_num << endl;
    cerr << "Usage: " << tkn.str << endl;
    exit(0);
  }
}

//This function is checking for the scan syntax.
node_t *A(){
  node_t *p = getNode("A");

  if(tkn.str == "scan"){
    tkn = scanner();
    if(tkn.token == IDENT_tk || tkn.token == NUM_tk){

      if(tkn.token == IDENT_tk)
        p->tk_id = tkn;
      else
        p->tk_num = tkn;

      tkn = scanner();
      return p;
    }
    else{
      cerr << "Fatal: An identifier or numeric value is not defined on line " << tkn.line_num << endl;
      cerr << "Usage: " << tkn.str << endl;
      exit(0);
    }
  }
  else{
    cerr << "Fatal: 'scan' not defined on line " << tkn.line_num << endl;
    cerr << "Usage: " << tkn.str << endl;
    exit(0);
  }
}

//This function is checking for the if syntax.
node_t *I(){
  node_t *p = getNode("I");

  if(tkn.str == "if"){
    tkn = scanner();
    if(tkn.str == "["){
      tkn = scanner();
      p->child1 = M();
      p->child2 = Z();
      p->child3 = M();

      if(tkn.str == "]"){
        tkn = scanner();
        p->child4 = T();
        return p;
      }
      else{
        cerr << "Fatal: ']' is missing in the if statement on line " << tkn.line_num << endl;
        cerr << "Usage: " << tkn.str << endl;
        exit(0);
      }
    }
    else{
      cerr << "Fatal: '[' is missing in the if statement on line " << tkn.line_num << endl;
      cerr << "Usage: " << tkn.str << endl;
      exit(0);
    }
  }
  else{
    cerr << "Fatal: 'if' not defined on line " << tkn.line_num << endl;
    cerr << "Usage: " << tkn.str << endl;
    exit(0);
  }
}

//This function is for checking the syntax for performing the operaitons.
node_t *M(){
  node_t *p = getNode("M");

  if(tkn.str == "&" || tkn.token == IDENT_tk || tkn.token == NUM_tk){
    p->child1 = H();

    if(tkn.str == "+" || tkn.str == "-" || tkn.str == "*" || tkn.str == "/"){
      p->tk_oper = tkn;
      tkn = scanner();
      p->child2 = M();
      return p;
    }
    else{
      return p;
    }
  }
  else{
    cerr << "Fatal: An identifier or a numeric value is not defined on line " << tkn.line_num << endl;
    cerr << "Usage: " << tkn.str << endl;
    exit(0);
  }
}

//THis function is checking for the operator.
node_t *Z(){
  node_t *p = getNode("Z");

  if(tkn.str == "<" || tkn.str == ">" || tkn.str == ":" || tkn.str == "=" || tkn.str == "=="){
    p->tk_oper = tkn;
    tkn = scanner();
    return p;
  }
  else{
    cerr << "Fatal: An operator is not defined on line " << tkn.line_num << endl;
    cerr << "Usage: " << tkn.str << endl;
    exit(0);
  }
}

//This function is checking for the ampersand.
node_t *H(){
  node_t *p = getNode("H");

  if(tkn.str == "&"){
    p->tk_delim = tkn;
    tkn = scanner();
    p->child1 = R();
    return p;
  }
  else{
    p->child1 = R();
    return p;
  }
}

//This function is checking for the identifier or numeric.
node_t *R(){
  node_t *p = getNode("R");

  if(tkn.token == IDENT_tk || tkn.token == NUM_tk){
    if(tkn.token == IDENT_tk)
      p->tk_id = tkn;
    else
      p->tk_num = tkn;

    tkn = scanner();
    return p;
  }
  else{
    cerr << "Fatal: An identifier or a number is not defined on line" << endl;
    cerr << "Usage: " << tkn.str << endl;
    exit(0);
  }
}

//Here is where the string gets appended
//Got that code from https://www.geeksforgeeks.org/how-to-convert-a-single-character-to-string-in-cpp/
string getString(char x){
  string str(1, x);
  return str;
}

string check_err_and_append(int next_state, char ch, string tkn_str){

  string tmp;
  //If there is an error encountered in a token, the program will be terminated
  if(next_state < 0){
    cerr << "Fatal: Unwanted definition occured." << endl;
    exit(0);
  }

  //Here is where the string gets appended incrementally to form a token
  //if a token is not detected and the next state is not a white space
  if((next_state < 1000) && (next_state != 1)){
    tmp = getString(ch);
    tkn_str.append(tmp);
  }

  return tkn_str;
}
