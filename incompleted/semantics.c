
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantics.h"
#include "error.h"

extern SymTab* symtab;
extern Token* currentToken;

Object* lookupObject(char *name) {
  Scope* scope = symtab->currentScope;
  Object* obj;

  while (scope != NULL) {
    obj = findObject(scope->objList, name);
    if (obj != NULL) return obj;
    scope = scope->outer;
  }
  obj = findObject(symtab->globalObjectList, name);
  if (obj != NULL) return obj;
  return NULL;
}

void checkFreshIdent(char *name) {
  if (findObject(symtab->currentScope->objList, name) != NULL)
    error(ERR_DUPLICATE_IDENT, currentToken->lineNo, currentToken->colNo);
}

Object* checkDeclaredIdent(char* name) {
  Object* obj = lookupObject(name);
  if (obj == NULL) {
    error(ERR_UNDECLARED_IDENT,currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object* checkDeclaredConstant(char* name) {
  Object* obj = lookupObject(name);
  if (obj == NULL)
    error(ERR_UNDECLARED_CONSTANT,currentToken->lineNo, currentToken->colNo);
  if (obj->kind != OBJ_CONSTANT)
    error(ERR_INVALID_CONSTANT,currentToken->lineNo, currentToken->colNo);

  return obj;
}

Object* checkDeclaredType(char* name) {
  Object* obj = lookupObject(name);
  if (obj == NULL)
    error(ERR_UNDECLARED_TYPE,currentToken->lineNo, currentToken->colNo);
  if (obj->kind != OBJ_TYPE)
    error(ERR_INVALID_TYPE,currentToken->lineNo, currentToken->colNo);

  return obj;
}

Object* checkDeclaredVariable(char* name) {
  Object* obj = lookupObject(name);
  if (obj == NULL)
    error(ERR_UNDECLARED_VARIABLE,currentToken->lineNo, currentToken->colNo);
  if (obj->kind != OBJ_VARIABLE)
    error(ERR_INVALID_VARIABLE,currentToken->lineNo, currentToken->colNo);

  return obj;
}

Object* checkDeclaredFunction(char* name) {
  Object* obj = lookupObject(name);
  if (obj == NULL)
    error(ERR_UNDECLARED_FUNCTION,currentToken->lineNo, currentToken->colNo);
  if (obj->kind != OBJ_FUNCTION)
    error(ERR_INVALID_FUNCTION,currentToken->lineNo, currentToken->colNo);

  return obj;
}

Object* checkDeclaredProcedure(char* name) {
  Object* obj = lookupObject(name);
  if (obj == NULL)
    error(ERR_UNDECLARED_PROCEDURE,currentToken->lineNo, currentToken->colNo);
  if (obj->kind != OBJ_PROCEDURE)
    error(ERR_INVALID_PROCEDURE,currentToken->lineNo, currentToken->colNo);

  return obj;
}

Object* checkDeclaredLValueIdent(char* name) {
  Object* obj = lookupObject(name);
  if (obj == NULL)
    error(ERR_UNDECLARED_IDENT,currentToken->lineNo, currentToken->colNo);

  switch (obj->kind) {
    case OBJ_VARIABLE:
    case OBJ_PARAMETER:
      break;
    case OBJ_FUNCTION:
      if (obj != symtab->currentScope->owner) 
        error(ERR_INVALID_IDENT,currentToken->lineNo, currentToken->colNo);
      break;
    default:
      error(ERR_INVALID_IDENT,currentToken->lineNo, currentToken->colNo);
  }

  return obj;
}


void checkIntType(Type* type) {
  if(type != NULL && type->typeClass == TP_INT) {
    return;
  } else {
    error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
  }
}

void checkCharType(Type* type) {
  if(type != NULL && type->typeClass == TP_CHAR) {
    return;
  } else {
    error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
  }
}

// TODO:GK1
void checkStringType(Type* type) {      // Them ham check kieu du lieu string
  if(type != NULL && type->typeClass == TP_STRING) {
    return;
  } else {
    error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
  }
}
// TODO:GK1 
void checkDoubleType(Type* type) {    // Them ham check kieu du lieu double
  if(type != NULL && type->typeClass == TP_DOUBLE) {
    return;
  } else {
    error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
  }
}
// TODO:GK1
void checkNumberType(Type* type) {  // THem ham check kieu du lieu là int or double
  if(type != NULL && (type->typeClass == TP_DOUBLE || type->typeClass == TP_INT)) {
    return;
  } else {
    error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
  }
}

//TODO: GK3 
void checkLetterType(Type* type){ // tao ham check kieu du lieu string hoac char
  if(type != NULL && (type->typeClass == TP_CHAR || type->typeClass == TP_STRING)){
    return;
  }else{
    error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
  }
}

// TODO:GK1
void checkBasicType(Type* type) {   
  if(type != NULL && (type->typeClass == TP_INT || type->typeClass == TP_CHAR || type->typeClass == TP_STRING || type->typeClass == TP_DOUBLE)) {
    return;
  } else {
    error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
  }
}

void checkArrayType(Type* type) {
  if(type != NULL && type->typeClass == TP_ARRAY) {
    return;
  } else {
    error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
  }
}

void checkTypeEquality(Type* type1, Type* type2) {      // check 2 type is equal
  if(compareType(type1, type2) == 0) {
    error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
  }
}

// TODO:GK3
void checkTypeInAssignment(Type* type1, Type* type2) {               //Check 2 type in left and right of assignment
  if(compareTypeInAssignment(type1, type2) == 0) {
    error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
  }
}

// TODO:GK3 
void checkMultiTypeAssignment(TypeNode* list1, TypeNode* list2){      // Kiem tra gan ep kieu
  int size1 = countTypeNode(list1);
  int size2 = countTypeNode(list2);
  if (size1 != size2) {
    error(ERR_ASSIGN_MULTI_VAR, currentToken->lineNo, currentToken->colNo);
  } 

  TypeNode* l1 = list1;
  TypeNode* l2 = list2;

  while (l1 != NULL && l2 != NULL) {
    if(compareTypeInAssignment(l1->type, l2->type) == 0) {
      error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
    }
    l1 = l1->next;
    l2 = l2->next;
  }
}

// TODO:GK3   
void checkTypeInExpression(Type* type1, Type* type2) {       // So sanh 2 kieu co the cong duoc
  if(compareTypeInExpression(type1, type2) == 0) {
    error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
  }
}


