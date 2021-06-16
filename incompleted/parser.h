
#ifndef __PARSER_H__
#define __PARSER_H__
#include "token.h"
#include "symtab.h"

void scan(void);
void eat(TokenType tokenType);

void compileProgram(void);
void compileBlock(void);
void compileBlock2(void);
void compileBlock3(void);
void compileBlock4(void);
void compileBlock5(void);
void compileConstDecls(void);
void compileConstDecl(void);
void compileTypeDecls(void);
void compileTypeDecl(void);
void compileVarDecls(void);
void compileVarDecl(void);
void compileSubDecls(void);
void compileFuncDecl(void);
void compileProcDecl(void);
ConstantValue* compileUnsignedConstant(void);
ConstantValue* compileConstant(void);
ConstantValue* compileConstant2(void);
Type* compileType(void);
Type* compileBasicType(void);
void compileParams(void);
void compileParam(void);
void compileStatements(void);
void compileStatement(void);
Type* compileLValue(void);
void compileAssignSt(void);
void compileCallSt(void);
void compileGroupSt(void);
void compileIfSt(void);
void compileElseSt(void);
void compileWhileSt(void);
void compileForSt(void);// TODO: CK3 them cau lenh switch case
void compileDoWhileSt(void);        // TODO: GK2 them cau lenh do-while
void compileArgument(Object* param);
void compileArguments(ObjectNode* paramList);
void compileCondition(void);
Type* compileExpression(void);
Type* compileExpression2(void);
Type* compileExpression3(void);
Type* compileTerm(void);
void compileTerm2(void);
Type* compileExp(void);             // TODO: CK2: them phep lay mu
void compileExp2(void);             // TODO: CK2: them phep lay mu
Type* compileFactor(void);
Type* compileIndexes(Type* arrayType);

//TODO:GK3
TypeNode* compileListLVars();
TypeNode* compileListRExps();

int compile(char *fileName);

#endif
