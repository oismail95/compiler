#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include "assembly.h"
#include "node.h"
using namespace std;

static int tCount = 0;
static int lCount = 0;
static vector<string> id;
static vector<string> temp_str;
static int ct = 0;

void assembly(node_t *node, int level, ofstream &out){
  if(node == NULL)
    return;

  if(node->label == "V"){
    //Got that code snippet from https://www.geeksforgeeks.org/vector-in-cpp-stl/
    id.push_back(node->tk_id.str);

    assembly(node->child1, level + 1, out);
    assembly(node->child2, level + 1, out);
    assembly(node->child3, level + 1, out);
    assembly(node->child4, level + 1, out);
  }
  else if(node->label == "G")
    write2asm_G(node, out);
  else if(node->label == "A")
    write2asm_A(node, out);
  else if(node->label == "W")
    write2asm_W(node, out);
  else if(node->label == "E")
    write2asm_E(node, out);
  else if(node->label == "I")
    write2asm_I(node, out);
  else if(node->label == "G")
    write2asm_G(node, out);
  else{
    assembly(node->child1, level + 1, out);
    assembly(node->child2, level + 1, out);
    assembly(node->child3, level + 1, out);
    assembly(node->child4, level + 1, out);
  }
}

//Here is the nonterminal, 'M', for operations,
//load, and ampressand statemets using inorder
//Got that code snippet from https://www.geeksforgeeks.org/tree-traversals-inorder-preorder-and-postorder/
void write2asm_M(node_t *node, ofstream &out){
  string label;

  if(node == NULL)
    return;

  write2asm_M(node->child1, out);

  if(node->label == "R"){
    //If the first id or number exists, the 'LOAD ' id/number will be written
    //Else the id/number will be written after operation
    if(ct == 0){
      if(node->tk_id.str != "")
        out << "LOAD " << node->tk_id.str << endl;
      else if(node->tk_num.str != "")
        out << "LOAD " << node->tk_num.str << endl;
    }
    else{
      if(node->tk_id.str != "")
        out << node->tk_id.str << endl;
      else if(node->tk_num.str != "")
        out << node->tk_num.str << endl;
    }
    ct++;
  }

  //Here is where the ampressand operation gets written
  if(node->label == "H"){
    if(node->tk_delim.str == "&"){
      label = createLabel();
      label = "L" + label;

      out << "BRPOS " << label << endl;
      out << "MULT -1" << endl;
      out << label << ": NOOP" << endl;
    }
  }

  //Here is where the operation gets written to perform from
  //left to right using add, subtract, multiply, and divide
  if(node->label == "M"){
    if(node->tk_oper.str != ""){
      if(node->tk_oper.str == "+")
        out << "ADD ";
      else if(node->tk_oper.str == "-")
        out << "SUB ";
      else if(node->tk_oper.str == "*")
        out << "MULT ";
      else if(node->tk_oper.str == "/")
        out << "DIV ";
    }
  }

  write2asm_M(node->child2, out);
}

//Here is the nonterminal, 'A', for scan statements
void write2asm_A(node_t *node, ofstream &out){
  if(node->tk_id.str != "")
    out << "READ " << node->tk_id.str << endl;
  else if(node->tk_num.str != "")
    out << "READ " << node->tk_num.str << endl;
}

//Here is the nonterminal, 'W', for write statements
void write2asm_W(node_t *node, ofstream &out){
  string temp;

  temp = createTempVar();
  temp = "T" + temp;
  temp_str.push_back(temp);

  write2asm_M(node->child1, out);
  out << "STORE " << temp << endl;
  out << "WRITE " << temp << endl;
  ct = 0;
}

//Here is the nonterminal, 'E', for let statements
void write2asm_E(node_t *node, ofstream &out){
  write2asm_M(node->child1, out);
  out << "STORE " << node->tk_id.str << endl;
  ct = 0;
}

void write2asm_T(node_t *node, ofstream &out){
  if(node == NULL)
    return;

  if(node->label == "W")
    write2asm_W(node, out);
  else if(node->label == "A")
    write2asm_A(node, out);
  else if(node->label == "E")
    write2asm_E(node, out);
  else if(node->label == "I")
    write2asm_I(node, out);
  else if(node->label == "G")
    write2asm_G(node, out);
  else{
    write2asm_T(node->child1, out);
    write2asm_T(node->child2, out);
    write2asm_T(node->child3, out);
    write2asm_T(node->child4, out);
  }
}

//Here is the nonterminal, 'I', for if statements
void write2asm_I(node_t *node, ofstream &out){
  string temp;
  string label;
  string oper;

  //Performs the operation in the third child of nonterminal, 'M'
  write2asm_M(node->child3, out);
  ct = 0;

  temp = createTempVar();
  temp = "T" + temp;
  temp_str.push_back(temp);

  out << "STORE " << temp << endl;

  //Performs the operation in the first child of nonterminal, 'M'
  write2asm_M(node->child1, out);
  ct = 0;

  out << "SUB " << temp << endl;

  label = createLabel();
  label = "L" + label;

  //Grabs the operator in the second child
  oper = write2asm_Z(node->child2);

  if(oper == ">")
    out << "BRZNEG " << label << endl;
  else if(oper == "<")
    out << "BRZPOS " << label << endl;
  else if(oper == "=" || oper == "==" || oper == ":"){
    out << "BRNEG " << label << endl;
    out << "BRPOS " << label << endl;
  }
  else{
    cerr << "ERROR: An operator is not defined in the if statement" << endl;
    exit(0);
  }

  //Performs the operation in the fourth child of nonterminal, 'T'
  write2asm_T(node->child4, out);

  out << label << ": NOOP" << endl;
}

//Here is where nonterminal, 'G', is used for repeat statements
void write2asm_G(node_t *node, ofstream &out){
  string temp;
  string label_1;
  string label_2;
  string oper;

  label_1 = createLabel();
  label_1 = "L" + label_1;

  label_2 = createLabel();
  label_2 = "L" + label_2;

  out << label_1 << ": NOOP" << endl;

  //Performs the operation in the third child of nonterminal, 'M'
  write2asm_M(node->child3, out);
  ct = 0;

  temp = createTempVar();
  temp = "T" + temp;
  temp_str.push_back(temp);

  out << "STORE " << temp << endl;

  //Performs the operation in the first child of nonterminal, 'M'
  write2asm_M(node->child1, out);
  ct = 0;

  out << "SUB " << temp << endl;

  //Grabs the operator in the second child
  oper = write2asm_Z(node->child2);

  if(oper == ">")
    out << "BRZNEG " << label_2 << endl;
  else if(oper == "<")
    out << "BRZPOS " << label_2 << endl;
  else if(oper == "=" || oper == "==" || oper == ":"){
    out << "BRNEG " << label_2 << endl;
    out << "BRPOS " << label_2 << endl;
  }
  else{
    cerr << "ERROR: An operator is not defined in the repeat statement" << endl;
    exit(0);
  }

  //Performs the operation in the fourth child of nonterminal, 'T'
  write2asm_T(node->child4, out);

  out << "BR " << label_1 << endl;

  out << label_2 << ": NOOP" << endl;
}

//Got that code snippet from https://www.javatpoint.com/cpp-int-to-string
string createTempVar(){
  string temp;
  stringstream ss;

  tCount++;

  ss << tCount;
  ss >> temp;

  return temp;
}

//Got that code snippet from https://www.javatpoint.com/cpp-int-to-string
string createLabel(){
  string label;
  stringstream ss;

  lCount++;

  ss << lCount;
  ss >> label;

  return label;
}

//Here is where the operator is grabbed in the nonterminal, 'Z'
string write2asm_Z(node_t *node){
  if(node == NULL)
    return "";

  if(node->label == "Z")
    return node->tk_oper.str;

  write2asm_Z(node->child1);
  write2asm_Z(node->child2);
  write2asm_Z(node->child3);
  write2asm_Z(node->child4);
}

//Here is where the ids gets displayed into the end of the assembly file after 'STOP'
void endOfAsmFile(string *symbolTable, ofstream &out){
  string tempId;

  out << "STOP" << endl;

  //Got that code snippet from https://www.geeksforgeeks.org/vector-in-cpp-stl/
  for(int i = 0; i < id.size(); i++){
    out << symbolTable[i] << " 0"<< endl;
    tempId = symbolTable[i];
  }

  //The condition is places so that way the same id will not be placed again
  if(tempId != id[0])
    out << id[0] << " 0" << endl;

  //Got that code snippet from https://www.geeksforgeeks.org/vector-in-cpp-stl/
  for(int i = 0; i < temp_str.size(); i++){
    out << temp_str[i] << " 0" << endl;
  }
}
