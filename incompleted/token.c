
#include <stdlib.h>
#include <ctype.h>
#include "token.h"

// TODO:GK1
struct {
  char string[MAX_IDENT_LEN + 1];
  TokenType tokenType;
} keywords[KEYWORDS_COUNT] = {
  {"PROGRAM", KW_PROGRAM},
  {"CONST", KW_CONST},
  {"TYPE", KW_TYPE},
  {"VAR", KW_VAR},
  {"INTEGER", KW_INTEGER},
  {"CHAR", KW_CHAR},
  {"STRING", KW_STRING},    //Thêm keyword string vao trong bang keyword
  {"DOUBLE", KW_DOUBLE},    //Thêm keyword double vao trong bang keyword
  {"ARRAY", KW_ARRAY},
  {"OF", KW_OF},
  {"FUNCTION", KW_FUNCTION},
  {"PROCEDURE", KW_PROCEDURE},
  {"BEGIN", KW_BEGIN},
  {"END", KW_END},
  {"CALL", KW_CALL},
  {"IF", KW_IF},
  {"THEN", KW_THEN},
  {"ELSE", KW_ELSE},
  {"WHILE", KW_WHILE},
  {"DO", KW_DO},
  {"FOR", KW_FOR},
  {"TO", KW_TO},
  {"SWITCH", KW_SWITCH},      //gk2: them keyword switch vao trong bang keyword
  {"CASE", KW_CASE},          //gk2: them keyword case vao trong bang keyword
  {"DEFAULT", KW_DEFAULT},     //gk2: them keyword default vao trong bang keyword
  {"BREAK", KW_BREAK}
};

int keywordEq(char *kw, char *string) {
  while ((*kw != '\0') && (*string != '\0')) {
    if (*kw != *string) break;
    kw ++; string ++;
  }
  return ((*kw == '\0') && (*string == '\0'));
}

TokenType checkKeyword(char *string) {
  int i;
  for (i = 0; i < KEYWORDS_COUNT; i++)
    if (keywordEq(keywords[i].string, string)) 
      return keywords[i].tokenType;
  return TK_NONE;
}

Token* makeToken(TokenType tokenType, int lineNo, int colNo) {
  Token *token = (Token*)malloc(sizeof(Token));
  token->tokenType = tokenType;
  token->lineNo = lineNo;
  token->colNo = colNo;
  return token;
}

// TODO:GK1
char *tokenToString(TokenType tokenType) {
  switch (tokenType) {
  case TK_NONE: return "None";
  case TK_IDENT: return "an identification";
  case TK_NUMBER: return "a number";
  case TK_CHAR: return "a constant char";
  case TK_STRING: return "a string";          // GK1: Bổ sung case cho 4 Token vừa thêm vào
  case TK_DOUBLE: return "a double number";   // GK1: Bổ sung case cho 4 Token vừa thêm vào
  case TK_EOF: return "end of file";
  case KW_PROGRAM: return "keyword PROGRAM";
  case KW_CONST: return "keyword CONST";
  case KW_TYPE: return "keyword TYPE";
  case KW_VAR: return "keyword VAR";
  case KW_INTEGER: return "keyword INTEGER";
  case KW_STRING: return "keyword STRING";    // GK1: Bổ sung case cho 4 Token vừa thêm vào
  case KW_DOUBLE: return "keyword DOUBLE";    // GK1: Bổ sung case cho 4 Token vừa thêm vào
  case KW_CHAR: return "keyword CHAR";
  case KW_ARRAY: return "keyword ARRAY";
  case KW_OF: return "keyword OF";
  case KW_FUNCTION: return "keyword FUNCTION";
  case KW_PROCEDURE: return "keyword PROCEDURE";
  case KW_BEGIN: return "keyword BEGIN";
  case KW_END: return "keyword END";
  case KW_CALL: return "keyword CALL";
  case KW_IF: return "keyword IF";
  case KW_THEN: return "keyword THEN";
  case KW_ELSE: return "keyword ELSE";
  case KW_WHILE: return "keyword WHILE";
  case KW_DO: return "keyword DO";
  case KW_FOR: return "keyword FOR";
  case KW_TO: return "keyword TO";
  case KW_SWITCH: return "keyword SWITCH";     //gk2: bo sung case cho token switch vua them vao
  case KW_CASE: return "keyword CASE";         //gk2: bo sung case cho token case vua them vao
  case KW_DEFAULT: return "keyword DEFAULT";   //gk2: bo sung case cho token default vua them vao
  case KW_BREAK: return "keyword BREAK";       //gk2: bo sung case cho token break vua them vao

  case SB_SEMICOLON: return "\';\'";
  case SB_COLON: return "\':\'";
  case SB_PERIOD: return "\'.\'";
  case SB_COMMA: return "\',\'";
  case SB_ASSIGN: return "\':=\'";
  case SB_EQ: return "\'=\'";
  case SB_NEQ: return "\'!=\'";
  case SB_LT: return "\'<\'";
  case SB_LE: return "\'<=\'";
  case SB_GT: return "\'>\'";
  case SB_GE: return "\'>=\'";
  case SB_PLUS: return "\'+\'";
  case SB_MINUS: return "\'-\'";
  case SB_TIMES: return "\'*\'";
  case SB_SLASH: return "\'/\'";
  case SB_LPAR: return "\'(\'";
  case SB_RPAR: return "\')\'";
  case SB_LSEL: return "\'(.\'";
  case SB_RSEL: return "\'.)\'";
  case SB_MU: return "\'^'\'";
  default: return "";
  }
}
