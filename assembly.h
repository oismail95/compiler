#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include <iostream>
#include <string>
#include <fstream>
#include "node.h"
using namespace std;

void assembly(node_t *, int, ofstream &);
void write2asm_M(node_t *, ofstream &);
void write2asm_A(node_t *, ofstream &);
void write2asm_W(node_t *, ofstream &);
void write2asm_E(node_t *, ofstream &);
void write2asm_I(node_t *, ofstream &);
void write2asm_G(node_t *, ofstream &);
void write2asm_T(node_t *, ofstream &);
string createTempVar();
string createLabel();
string write2asm_Z(node_t *);
void endOfAsmFile(string *, ofstream &);

#endif
