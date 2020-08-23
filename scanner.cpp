#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "scanner.h"
#include "token.h"

//This table is used to compare each character and returns right next state number
static int fsa_table[8][19] = {{1, -1, 2, 4, 7, 7, 7, 7, 5, 7, 7, 8, 8, 8, 8, 8, 8, 8, 1005},
                               {-1, 3, 3, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                               {1001, 3, 3, 3, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001},
                               {1002, 1002, 1002, 4, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002},
                               {1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 6, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003},
                               {1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003},
                               {1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003},
                               {1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004}};

static vector<token> tk;

int fad_driver(char data, int state){

  //Decrements one to go on the right bound position
  if(state != 0)
    state--;

  //Here is where the first state is checked
  if(state == 0){
    if(isspace(data))
      return fsa_table[state][0];
    else if(isupper(data))
      return fsa_table[state][1];
    else if(islower(data))
      return fsa_table[state][2];
    else if(isdigit(data))
      return fsa_table[state][3];
    else if(data == '+')
      return fsa_table[state][4];
    else if(data == '-')
      return fsa_table[state][5];
    else if(data == '*')
      return fsa_table[state][6];
    else if(data == '/')
      return fsa_table[state][7];
    else if(data == '=')
      return fsa_table[state][8];
    else if(data == '<')
      return fsa_table[state][9];
    else if(data == '>')
      return fsa_table[state][10];
    else if(data == ':')
      return fsa_table[state][11];
    else if(data == '&')
      return fsa_table[state][12];
    else if(data == '#')
      return fsa_table[state][13];
    else if(data == '.')
      return fsa_table[state][14];
    else if(data == '[')
      return fsa_table[state][15];
    else if(data == ']')
      return fsa_table[state][16];
    else if(data == ',')
      return fsa_table[state][17];
    else if(data == '\0')
      return fsa_table[state][18];
  }

  //Here is where the second state is checked for creating an identifier token
  if(state == 1){
    if(isspace(data))
      return fsa_table[state][0];
    else if(isupper(data))
      return fsa_table[state][1];
    else if(islower(data))
      return fsa_table[state][2];
    else if(isdigit(data))
      return fsa_table[state][3];
    else if(data == '+')
      return fsa_table[state][4];
    else if(data == '-')
      return fsa_table[state][5];
    else if(data == '*')
      return fsa_table[state][6];
    else if(data == '/')
      return fsa_table[state][7];
    else if(data == '=')
      return fsa_table[state][8];
    else if(data == '<')
      return fsa_table[state][9];
    else if(data == '>')
      return fsa_table[state][10];
    else if(data == ':')
      return fsa_table[state][11];
    else if(data == '&')
      return fsa_table[state][12];
    else if(data == '#')
      return fsa_table[state][13];
    else if(data == '.')
      return fsa_table[state][14];
    else if(data == '[')
      return fsa_table[state][15];
    else if(data == ']')
      return fsa_table[state][16];
    else if(data == ',')
      return fsa_table[state][17];
    else if(data == '\0')
      return fsa_table[state][18];
  }

  //Here is where the third state is checked for the process of identifier token
  if(state == 2){
    if(isspace(data))
      return fsa_table[state][0];
    else if(isupper(data))
      return fsa_table[state][1];
    else if(islower(data))
      return fsa_table[state][2];
    else if(isdigit(data))
      return fsa_table[state][3];
    else if(data == '+')
      return fsa_table[state][4];
    else if(data == '-')
      return fsa_table[state][5];
    else if(data == '*')
      return fsa_table[state][6];
    else if(data == '/')
      return fsa_table[state][7];
    else if(data == '=')
      return fsa_table[state][8];
    else if(data == '<')
      return fsa_table[state][9];
    else if(data == '>')
      return fsa_table[state][10];
    else if(data == ':')
      return fsa_table[state][11];
    else if(data == '&')
      return fsa_table[state][12];
    else if(data == '#')
      return fsa_table[state][13];
    else if(data == '.')
      return fsa_table[state][14];
    else if(data == '[')
      return fsa_table[state][15];
    else if(data == ']')
      return fsa_table[state][16];
    else if(data == ',')
      return fsa_table[state][17];
    else if(data == '\0')
      return fsa_table[state][18];
  }

  //Here is where the fourth state is checked for a numeric token
  if(state == 3){
    if(isspace(data))
      return fsa_table[state][0];
    else if(isupper(data))
      return fsa_table[state][1];
    else if(islower(data))
      return fsa_table[state][2];
    else if(isdigit(data))
      return fsa_table[state][3];
    else if(data == '+')
      return fsa_table[state][4];
    else if(data == '-')
      return fsa_table[state][5];
    else if(data == '*')
      return fsa_table[state][6];
    else if(data == '/')
      return fsa_table[state][7];
    else if(data == '=')
      return fsa_table[state][8];
    else if(data == '<')
      return fsa_table[state][9];
    else if(data == '>')
      return fsa_table[state][10];
    else if(data == ':')
      return fsa_table[state][11];
    else if(data == '&')
      return fsa_table[state][12];
    else if(data == '#')
      return fsa_table[state][13];
    else if(data == '.')
      return fsa_table[state][14];
    else if(data == '[')
      return fsa_table[state][15];
    else if(data == ']')
      return fsa_table[state][16];
    else if(data == ',')
      return fsa_table[state][17];
    else if(data == '\0')
      return fsa_table[state][18];
  }

  //Here is where the fifth state is checked to check the equal operator
  if(state == 4){
    if(isspace(data))
      return fsa_table[state][0];
    else if(isupper(data))
      return fsa_table[state][1];
    else if(islower(data))
      return fsa_table[state][2];
    else if(isdigit(data))
      return fsa_table[state][3];
    else if(data == '+')
      return fsa_table[state][4];
    else if(data == '-')
      return fsa_table[state][5];
    else if(data == '*')
      return fsa_table[state][6];
    else if(data == '/')
      return fsa_table[state][7];
    else if(data == '=')
      return fsa_table[state][8];
    else if(data == '<')
      return fsa_table[state][9];
    else if(data == '>')
      return fsa_table[state][10];
    else if(data == ':')
      return fsa_table[state][11];
    else if(data == '&')
      return fsa_table[state][12];
    else if(data == '#')
      return fsa_table[state][13];
    else if(data == '.')
      return fsa_table[state][14];
    else if(data == '[')
      return fsa_table[state][15];
    else if(data == ']')
      return fsa_table[state][16];
    else if(data == ',')
      return fsa_table[state][17];
    else if(data == '\0')
      return fsa_table[state][18];
  }

  //Here is where the sixth state is checked for the double equal operator
  if(state == 5){
    if(isspace(data))
      return fsa_table[state][0];
    else if(isupper(data))
      return fsa_table[state][1];
    else if(islower(data))
      return fsa_table[state][2];
    else if(isdigit(data))
      return fsa_table[state][3];
    else if(data == '+')
      return fsa_table[state][4];
    else if(data == '-')
      return fsa_table[state][5];
    else if(data == '*')
      return fsa_table[state][6];
    else if(data == '/')
      return fsa_table[state][7];
    else if(data == '=')
      return fsa_table[state][8];
    else if(data == '<')
      return fsa_table[state][9];
    else if(data == '>')
      return fsa_table[state][10];
    else if(data == ':')
      return fsa_table[state][11];
    else if(data == '&')
      return fsa_table[state][12];
    else if(data == '#')
      return fsa_table[state][13];
    else if(data == '.')
      return fsa_table[state][14];
    else if(data == '[')
      return fsa_table[state][15];
    else if(data == ']')
      return fsa_table[state][16];
    else if(data == ',')
      return fsa_table[state][17];
    else if(data == '\0')
      return fsa_table[state][18];
  }

  //Here is where the seventh state is checked for other operators
  if(state == 6){
    if(isspace(data))
      return fsa_table[state][0];
    else if(isupper(data))
      return fsa_table[state][1];
    else if(islower(data))
      return fsa_table[state][2];
    else if(isdigit(data))
      return fsa_table[state][3];
    else if(data == '+')
      return fsa_table[state][4];
    else if(data == '-')
      return fsa_table[state][5];
    else if(data == '*')
      return fsa_table[state][6];
    else if(data == '/')
      return fsa_table[state][7];
    else if(data == '=')
      return fsa_table[state][8];
    else if(data == '<')
      return fsa_table[state][9];
    else if(data == '>')
      return fsa_table[state][10];
    else if(data == ':')
      return fsa_table[state][11];
    else if(data == '&')
      return fsa_table[state][12];
    else if(data == '#')
      return fsa_table[state][13];
    else if(data == '.')
      return fsa_table[state][14];
    else if(data == '[')
      return fsa_table[state][15];
    else if(data == ']')
      return fsa_table[state][16];
    else if(data == ',')
      return fsa_table[state][17];
    else if(data == '\0')
      return fsa_table[state][18];
  }

  //Here is where the seventh state is checked for the delimeter
  if(state == 7){
    if(isspace(data))
      return fsa_table[state][0];
    else if(isupper(data))
      return fsa_table[state][1];
    else if(islower(data))
      return fsa_table[state][2];
    else if(isdigit(data))
      return fsa_table[state][3];
    else if(data == '+')
      return fsa_table[state][4];
    else if(data == '-')
      return fsa_table[state][5];
    else if(data == '*')
      return fsa_table[state][6];
    else if(data == '/')
      return fsa_table[state][7];
    else if(data == '=')
      return fsa_table[state][8];
    else if(data == '<')
      return fsa_table[state][9];
    else if(data == '>')
      return fsa_table[state][10];
    else if(data == ':')
      return fsa_table[state][11];
    else if(data == '&')
      return fsa_table[state][12];
    else if(data == '#')
      return fsa_table[state][13];
    else if(data == '.')
      return fsa_table[state][14];
    else if(data == '[')
      return fsa_table[state][15];
    else if(data == ']')
      return fsa_table[state][16];
    else if(data == ',')
      return fsa_table[state][17];
    else if(data == '\0')
      return fsa_table[state][18];
  }
}

void call_token(string token, int line_no, int tkn_state){

  struct token tokenStruct;

  if(tkn_state == 1001){
    //Displays an error for having an identifier token longer than 8 characters
    if(token.length() > 8){
      cerr << "Fatal: Length of the identifier token has exceeded" << endl;
      cerr << "Usage: " << token << endl;
      exit(0);
    }

    //If the token is a keyword, the keyword will be displayed. Else the identifier will be displayed.
      if(token == "begin" || token == "end" || token == "repeat" || token == "if" || token == "then"
        || token == "let" || token == "void" || token == "var" || token == "return" || token == "scan"
        || token == "write" || token == "program"){
        tokenStruct.token = KWD_tk;
      }
      else
        tokenStruct.token = IDENT_tk;
  }
  else if(tkn_state == 1002){
    //Displays an error for having a numeric token longer than 8 characters
    if(token.length() > 8){
      cerr << "Fatal: Length of the numeric token has exceeded" << endl;
      cerr << "Usage: " << token << endl;
      exit(0);
    }

    tokenStruct.token = NUM_tk;
  }
  else if(tkn_state == 1003)
    tokenStruct.token = OPER_tk;
  else if(tkn_state == 1004)
    tokenStruct.token = DELIM_tk;
  else if(tkn_state == 1005)
    tokenStruct.token = EOF_tk;

  tokenStruct.str = token;
  tokenStruct.line_num = line_no;
  tk.push_back(tokenStruct);
}

token scanner(){
  struct token tempToken;
  vector<token>::iterator it;

  tempToken = tk.front();
  if (!tk.empty()){
  	it = tk.begin();
  	tk.erase(it);
  }
  return tempToken;
}
