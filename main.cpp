//Omar Ismail
//November 29, 2019
//CMP SCI 4280
//Instructor: Sharlee Climer

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cctype>
#include <cstring>
#include <sstream>
#include "token.h"
#include "parser.h"
#include "node.h"
#include "checkSem.h"
#include "assembly.h"

const int CHAR_SIZE = 1000;

using namespace std;
int main(int argc, char *argv[]){

  ifstream inFile;
  ofstream out;
  char amtFiles[CHAR_SIZE];
  string input;
  const char *fName;
  int fType;
  char data;
  bool comment;
  const char *filterFile;
  node_t *root;
  char asmFile[CHAR_SIZE];
  string *symbolTable;

  //Here is where the stdin is used.
  if(argc == 1){
    out.open("out.txt");

    cout << "Please enter the input: " << endl;
    cin >> input;

    while(!feof(stdin)){
      out << input << " ";
      cin >> input;
    }

    out << "\n";
    out.close();

    cout << endl;
    fType = 0;
  }

  //Here is where the file is taken to read.
  if(argc == 2){
    inFile.open(argv[1]);

    if(inFile.fail()){
      cerr << "Fatal: Cannot open file" << endl;
      cerr << "Usage: " << argv[1] << endl;
      exit(0);
    }
    inFile.close();
    fType = 1;
  }

  //Here is if more than one files or contents are used, an error will occur.
  if(argc > 2){
    for(int i = 0; i < argc; i++){
      if(i == 0){
        strcpy(amtFiles, argv[i]);
        strcat(amtFiles, " ");
      }
      else{
        strcat(amtFiles, argv[i]);
        strcat(amtFiles, " ");
      }
    }

    cerr << "Fatal: Used more than two files" << endl;
    cerr << "Usage: " << amtFiles  << endl;
    exit(0);
  }

  if(fType == 0)
    fName = "out.txt";
  else
    fName = argv[1];

  comment = false;
  filterFile = "filter.txt";

  inFile.open(fName);
  out.open(filterFile);

  //Got that code from https://www.youtube.com/watch?v=qI-tUsujrCU
  //Checks if there characters that are unwanted in the file
  //and removes comments by outputting it in a filter file
  while(!inFile.eof()){

    //Adds in white spaces to not miss it in the comments
    inFile >> noskipws  >> data;

    if(!inFile.eof()){
      if(isalpha(data) || isdigit(data) || isspace(data) || data == 10
        || data == 35 || data == 37 || data == 38 || (data >= 42 && data <= 47)
        || data == 58 || (data >= 60 && data <= 62) || data == 91 || data == 93){
        if(data == '%')
          comment = true;

        if(!comment)
          out << data;

        while(comment){
          inFile >> data;
          if(data == '%')
            comment = false;
        }
      }
      else{
        cerr << "Fatal: An unallowed character is in the file" << endl;
        cerr << "Usage: " << data << endl;
        exit(0);
      }
    }
  }

  inFile.close();
  out.close();

  //Builds the parse tree
  root = parser(inFile, filterFile);

  //Here is where the semantics gets checked on variables
  checkSem(root, 1, 0);

  //Here is where the symbol table gets retrieved
  symbolTable = getSymbolTable();

  //Here is where the file name is set up for assembly
  if(fType == 0)
    strcpy(asmFile, "kb.asm");
  else{
    strcpy(asmFile, argv[1]);
    strcat(asmFile, ".asm");
  }

  //Here is where the assembly file gets created
  out.open(asmFile);
  assembly(root, 1, out);

  //Here is where the contents from the end of the file gets displayed
  endOfAsmFile(symbolTable, out);
  out.close();

  cout << "Assembly File: " << asmFile << endl;

  return 0;
}
