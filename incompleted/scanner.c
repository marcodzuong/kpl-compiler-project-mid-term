
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"
#include "scanner.h"


extern int lineNo;
extern int colNo;
extern int currentChar;

extern CharCode charCodes[];

/***************************************************************/

 
void skipBlank() {
  while ((currentChar != EOF) && (charCodes[currentChar] == CHAR_SPACE))
    readChar();
}

void skipComment() {
  int state = 0;
  while ((currentChar != EOF) && (state < 2)) {
    switch (charCodes[currentChar]) {
    case CHAR_TIMES:
      state = 1;
      break;
    case CHAR_RPAR:
      if (state == 1) state = 2;
      else state = 0;
      break;
    default:
      state = 0;
    }
    readChar();
  }
  if (state != 2) 
    error(ERR_END_OF_COMMENT, lineNo, colNo);
}


Token* readIdentKeyword(void) {
  Token *token = makeToken(TK_NONE, lineNo, colNo);
  int count = 1;

  token->string[0] = toupper((char)currentChar);
  readChar();

  while ((currentChar != EOF) && 
	 ((charCodes[currentChar] == CHAR_LETTER) || (charCodes[currentChar] == CHAR_DIGIT))) {
    if (count <= MAX_IDENT_LEN) token->string[count++] = toupper((char)currentChar);
    readChar();
  }
  
  if (count > MAX_IDENT_LEN) {
    error(ERR_IDENT_TOO_LONG, token->lineNo, token->colNo);
    return token;
  }

  token->string[count] = '\0';
  token->tokenType = checkKeyword(token->string);

  if (token->tokenType == TK_NONE)
    token->tokenType = TK_IDENT;

  return token;
}

// TODO: 
Token* readNumber(void) {       // Sua lai truong hop doc number kieu int or kieu double
  Token *token = makeToken(TK_NONE, lineNo, colNo);
  int count = 0;
  int floatPoint = 0;          // dem so luong dau cham 

  while ((currentChar != EOF) && (charCodes[currentChar] == CHAR_DIGIT || charCodes[currentChar] == CHAR_PERIOD)) {
    if(charCodes[currentChar] == CHAR_PERIOD) floatPoint += 1;
    token->string[count++] = (char)currentChar;
    readChar();
  }

  token->string[count] = '\0';
  if (floatPoint == 0) {
    token->tokenType = TK_NUMBER;
    token->value = atoi(token->string);
  } else if(floatPoint == 1) {
    token->tokenType = TK_DOUBLE;
    token->dvalue = atof(token->string); 
  } else {
    error(ERR_INVALID_DOUBLE, token->lineNo, token->colNo);
  }
  return token;
}


// TODO:
Token* readConstChar(void) {    // Chinh ham thanh doc string or char
  Token *token = makeToken(TK_NONE, lineNo, colNo);
  int count = 0;
  readChar();
  if (currentChar == EOF) {
    token->tokenType = TK_NONE;
    error(ERR_INVALID_CONSTANT_CHAR, token->lineNo, token->colNo);
    return token;
  }

  while(charCodes[currentChar] != CHAR_SINGLEQUOTE){
    if (currentChar == EOF || currentChar == '\n'){
      error(ERR_INVALID_CONSTANT_CHAR, token->lineNo, token->colNo);
      return token;
    }
    token->string[count++] = (char)currentChar;
    readChar();
  }
  
  // while ((currentChar != EOF) && (charCodes[currentChar] == CHAR_LETTER || charCodes[currentChar] == CHAR_DIGIT || charCodes[currentChar] == CHAR_SPACE)) {
  //   token->string[count++] = (char)currentChar;
  //   readChar();
  // }
  // token->string[count] = '\0';

  // if (currentChar == EOF) {
  //   token->tokenType = TK_NONE;
  //   error(ERR_INVALID_CONSTANT_CHAR, token->lineNo, token->colNo);
  //   return token;
  // }

  if (charCodes[currentChar] == CHAR_SINGLEQUOTE) {
    readChar();
    if(count > 1) {
      token->tokenType = TK_STRING;
    } else {
      token->tokenType = TK_CHAR;
    }
    return token;
  } else {
    token->tokenType = TK_NONE;
    error(ERR_INVALID_CONSTANT_CHAR, token->lineNo, token->colNo);
    return token;
  }
}

Token* getToken(void) {
  Token *token;
  int ln, cn;

  if (currentChar == EOF) 
    return makeToken(TK_EOF, lineNo, colNo);

  switch (charCodes[currentChar]) {
  case CHAR_SPACE: skipBlank(); return getToken();        
  case CHAR_LETTER: return readIdentKeyword();          // Gap ky tu thi vao ham doc Ident để tạo ident va gan tokenType, cai nay tra ve 1 KW or 1 ident
  case CHAR_DIGIT: return readNumber();                 // Neu gap so thi doc so vao day token 
  case CHAR_PLUS: 
    token = makeToken(SB_PLUS, lineNo, colNo);          // Tao token SB_PLUS  (+)
    readChar();                                         // doc sang currentChar tiep theo
    return token;
  case CHAR_MINUS:                                      // Tao token SB_MINUS  (-)
    token = makeToken(SB_MINUS, lineNo, colNo);
    readChar(); 
    return token;

  //TODO: xu ly phan mu
  case CHAR_TIMES:   
    ln = lineNo;
    cn = colNo;
    readChar();
    if((currentChar != EOF) && (charCodes[currentChar]) == CHAR_TIMES){
      readChar();
      return makeToken(SB_MU, ln, cn);                  //tao token SB_MU
    }else return makeToken(SB_TIMES, lineNo, colNo);

  case CHAR_SLASH:                                      // Tao token SB_SLASH  (/)
    token = makeToken(SB_SLASH, lineNo, colNo);         
    readChar(); 
    return token;
  case CHAR_LT:                                         // Đọc <
    ln = lineNo;
    cn = colNo;
    readChar();
    if ((currentChar != EOF) && (charCodes[currentChar] == CHAR_EQ)) {        // Tạo token SB_LE (<=)
      readChar();
      return makeToken(SB_LE, ln, cn);
    } else return makeToken(SB_LT, ln, cn);                                   // Tạo token SB_LT (<)
  case CHAR_GT:                                         // Đọc >
    ln = lineNo;
    cn = colNo;
    readChar();
    if ((currentChar != EOF) && (charCodes[currentChar] == CHAR_EQ)) {        // Tạo token SB_GE  (>=)
      readChar();
      return makeToken(SB_GE, ln, cn);
    } else return makeToken(SB_GT, ln, cn);                                   // Tao token SB_GT(>)
  case CHAR_EQ:                                                               // Tạo token SB_EQ (=)
    token = makeToken(SB_EQ, lineNo, colNo);
    readChar(); 
    return token;
  case CHAR_EXCLAIMATION:                                                   // Đọc !
    ln = lineNo;
    cn = colNo;
    readChar();
    if ((currentChar != EOF) && (charCodes[currentChar] == CHAR_EQ)) {      // Tạo token SB_NEQ (!=)
      readChar();
      return makeToken(SB_NEQ, ln, cn);
    } else {                                                              // Dọc được dâus ! mà ko có = thì báo loi
      token = makeToken(TK_NONE, ln, cn);
      error(ERR_INVALID_SYMBOL, ln, cn);
      return token;
    }
  case CHAR_COMMA:                                                        // Tạo token SB_COMMA (,)
    token = makeToken(SB_COMMA, lineNo, colNo);
    readChar(); 
    return token;
  
  //TODO: neu gap dau chem check xem gtri tiep theo phai so khong => readNumber    
  // neu khong thi tra ve dau cham
  case CHAR_PERIOD:                                                       // Đọc .
    ln = lineNo;
    cn = colNo;
    readChar();
    if ((currentChar != EOF) && (charCodes[currentChar] == CHAR_RPAR)) {    // Tạo token SB_RSEL  (.))
      readChar();
      return makeToken(SB_RSEL, ln, cn);
    }else if((currentChar != EOF) && (charCodes[currentChar] == CHAR_DIGIT)){
      readChar();
      return readNumber();
    }
     else return makeToken(SB_PERIOD, ln, cn);                            // Tạo token SB_PERIOD (.)
  case CHAR_SEMICOLON:                                                     // Tạo token SB_SEMICOLON (;)
    token = makeToken(SB_SEMICOLON, lineNo, colNo);
    readChar(); 
    return token;
  case CHAR_COLON:                                                        // Đọc :
    ln = lineNo;
    cn = colNo;
    readChar();
    if ((currentChar != EOF) && (charCodes[currentChar] == CHAR_EQ)) {      // Tạo token SB_ASSIGN (:=)
      readChar();
      return makeToken(SB_ASSIGN, ln, cn);
    } else return makeToken(SB_COLON, ln, cn);                             // Tạo token SB_COLON (:)
  case CHAR_SINGLEQUOTE: return readConstChar();                            // Dọc 1 hang so Char
  case CHAR_LPAR:                                                           // Đọc (
    ln = lineNo;
    cn = colNo;
    readChar();

    if (currentChar == EOF)                                               // Tạo token SB_LPAR (()
      return makeToken(SB_LPAR, ln, cn);

    switch (charCodes[currentChar]) {
    case CHAR_PERIOD:                                                      // Tạo token SB_LSEL ((.)
      readChar();
      return makeToken(SB_LSEL, ln, cn);
    case CHAR_TIMES:                                                      // Đọc * => (*  => Đọc comment đến khi gặp *) Và bỏ qua
      readChar();
      skipComment();
      return getToken();
    default:
      return makeToken(SB_LPAR, ln, cn);                                // Tạo token SB_LPAR (()
    }
  case CHAR_RPAR:                                                       // Tạo token SB_RPAR ())
    token = makeToken(SB_RPAR, lineNo, colNo);
    readChar(); 
    return token;
  default:                                                            // Loi doc token
    token = makeToken(TK_NONE, lineNo, colNo);
    error(ERR_INVALID_SYMBOL, lineNo, colNo);
    readChar(); 
    return token;
  }
}

Token* getValidToken(void) {
  Token *token = getToken();
  while (token->tokenType == TK_NONE) {
    free(token);
    token = getToken();
  }
  return token;
}


/******************************************************************/

// TODO:GK1
void printToken(Token *token) {

  printf("%d-%d:", token->lineNo, token->colNo);

  switch (token->tokenType) {
  case TK_NONE: printf("TK_NONE\n"); break;
  case TK_IDENT: printf("TK_IDENT(%s)\n", token->string); break;
  case TK_NUMBER: printf("TK_NUMBER(%s)\n", token->string); break;
  case TK_CHAR: printf("TK_CHAR(\'%s\')\n", token->string); break; 
  case TK_DOUBLE: printf("TK_DOUBLE(\'%s\')\n", token->string); break;    // GK1: in token ra man hinh
  case TK_STRING: printf("TK_STRING(\'%s\')\n", token->string); break;    // GK1: in token ra man hinh
  case TK_EOF: printf("TK_EOF\n"); break;
  
  case KW_PROGRAM: printf("KW_PROGRAM\n"); break;
  case KW_CONST: printf("KW_CONST\n"); break;
  case KW_TYPE: printf("KW_TYPE\n"); break;
  case KW_VAR: printf("KW_VAR\n"); break;
  case KW_INTEGER: printf("KW_INTEGER\n"); break;
  case KW_CHAR: printf("KW_CHAR\n"); break; 
  case KW_STRING: printf("KW_STRING\n"); break;   // GK1: in token ra man hinh
  case KW_DOUBLE: printf("KW_DOUBLE\n"); break;   // GK1: in token ra man hinh
  case KW_ARRAY: printf("KW_ARRAY\n"); break;
  case KW_OF: printf("KW_OF\n"); break;
  case KW_FUNCTION: printf("KW_FUNCTION\n"); break;
  case KW_PROCEDURE: printf("KW_PROCEDURE\n"); break;
  case KW_BEGIN: printf("KW_BEGIN\n"); break;
  case KW_END: printf("KW_END\n"); break;
  case KW_CALL: printf("KW_CALL\n"); break;
  case KW_IF: printf("KW_IF\n"); break;
  case KW_THEN: printf("KW_THEN\n"); break;
  case KW_ELSE: printf("KW_ELSE\n"); break;
  case KW_WHILE: printf("KW_WHILE\n"); break;
  case KW_DO: printf("KW_DO\n"); break;
  case KW_FOR: printf("KW_FOR\n"); break;
  case KW_TO: printf("KW_TO\n"); break;
  case KW_SWITCH: printf("KW_SWITCH\n");break;  // gk2: in token switch ra man hinh
  case KW_CASE: printf("KW_CASE");break;        // gk2: in token case ra man hinh
  case KW_DEFAULT: printf("KW_DEFAULT");break;  // gk2: in token default ra man hinh
  case KW_BREAK: printf("KW_BREAK");break;      // gk2: in token break ra man hinh

  case SB_SEMICOLON: printf("SB_SEMICOLON\n"); break;
  case SB_COLON: printf("SB_COLON\n"); break;
  case SB_PERIOD: printf("SB_PERIOD\n"); break;
  case SB_COMMA: printf("SB_COMMA\n"); break;
  case SB_ASSIGN: printf("SB_ASSIGN\n"); break;
  case SB_EQ: printf("SB_EQ\n"); break;
  case SB_NEQ: printf("SB_NEQ\n"); break;
  case SB_LT: printf("SB_LT\n"); break;
  case SB_LE: printf("SB_LE\n"); break;
  case SB_GT: printf("SB_GT\n"); break;
  case SB_GE: printf("SB_GE\n"); break;
  case SB_PLUS: printf("SB_PLUS\n"); break;
  case SB_MINUS: printf("SB_MINUS\n"); break;
  case SB_TIMES: printf("SB_TIMES\n"); break;
  case SB_SLASH: printf("SB_SLASH\n"); break;
  case SB_LPAR: printf("SB_LPAR\n"); break;
  case SB_RPAR: printf("SB_RPAR\n"); break;
  case SB_LSEL: printf("SB_LSEL\n"); break;
  case SB_RSEL: printf("SB_RSEL\n"); break;
  case SB_MU: printf("SB_MU\n"); break;         //gk1: in tolen mu ra man hinh
  }
}

