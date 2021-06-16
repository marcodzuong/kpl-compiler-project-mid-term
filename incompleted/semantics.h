

#ifndef __SEMANTICS_H__
#define __SEMANTICS_H__

#include "symtab.h"

void checkFreshIdent(char *name);
Object* checkDeclaredIdent(char *name);
Object* checkDeclaredConstant(char *name);
Object* checkDeclaredType(char *name);
Object* checkDeclaredVariable(char *name);
Object* checkDeclaredFunction(char *name);
Object* checkDeclaredProcedure(char *name);
Object* checkDeclaredLValueIdent(char *name);

void checkIntType(Type* type);
void checkCharType(Type* type);
void checkStringType(Type* type);           // GK1: Check kieu du lieu là string ko
void checkDoubleType(Type* type);           // GK1: Check kieu du lieu la double ko
void checkNumberType(Type* type);           // GK1: Check du lieu la so 
void checkLetterType(Type* type);


void checkArrayType(Type* type);
void checkBasicType(Type* type);
void checkTypeEquality(Type* type1, Type* type2);                        // check type 2 type is equal

// TODO:GK3
void checkTypeInAssignment(Type* type1, Type* type2);                    // GK3: Check type left and right in assignment
void checkMultiTypeAssignment(TypeNode* list1, TypeNode* list2);         // GK3: Check type for multi assignment
void checkTypeInExpression(Type* type1, Type* type2);                    // GK3: Check type in expression
#endif
