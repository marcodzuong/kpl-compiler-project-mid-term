

#ifndef __TOKEN_H__
#define __TOKEN_H__

#define MAX_IDENT_LEN 15
// TODO:
#define KEYWORDS_COUNT 26    
#define MAX_LENGTH 255 

// TODO:
typedef enum {
  TK_NONE, TK_IDENT, TK_NUMBER, TK_CHAR, TK_EOF,
  TK_STRING, TK_DOUBLE,     
  
  KW_PROGRAM, KW_CONST, KW_TYPE, KW_VAR,
  KW_STRING, KW_DOUBLE,     
  KW_INTEGER, KW_CHAR, KW_ARRAY, KW_OF, 
  KW_FUNCTION, KW_PROCEDURE,
  KW_BEGIN, KW_END, KW_CALL,
  KW_IF, KW_THEN, KW_ELSE,
  KW_WHILE, KW_DO, KW_FOR, KW_TO,
  KW_SWITCH, KW_CASE, KW_DEFAULT, KW_BREAK,            //bai2: bo sung tu khoa switch, case, default,break

  SB_SEMICOLON, SB_COLON, SB_PERIOD, SB_COMMA,
  SB_ASSIGN, SB_EQ, SB_NEQ, SB_LT, SB_LE, SB_GT, SB_GE,
  SB_PLUS, SB_MINUS, SB_TIMES, SB_SLASH,
  SB_LPAR, SB_RPAR, SB_LSEL, SB_RSEL, SB_MU           //bai1: bo sung phep luy thua
} TokenType; 


// TODO:GK1
typedef struct {
  char string[MAX_IDENT_LEN + 1];
  int lineNo, colNo;
  TokenType tokenType;
  union {
    int value;
    double dvalue;                            // bo sung de luu gia tri double
  };
} Token;

TokenType checkKeyword(char *string);
Token* makeToken(TokenType tokenType, int lineNo, int colNo);
char *tokenToString(TokenType tokenType);


#endif
