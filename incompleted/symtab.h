

#ifndef __SYMTAB_H__
#define __SYMTAB_H__

#include "token.h"

// TODO:GK1
enum TypeClass {
  TP_INT,
  TP_CHAR,
  TP_ARRAY,
  TP_STRING,        // GK1: Thêm vào 2 TypeClass cho 2 kiêu dữ liệu mới
  TP_DOUBLE         // GK1: Thêm vào 2 TypeClass cho 2 kiêu dữ liệu mới
};

enum ObjectKind {
  OBJ_CONSTANT,
  OBJ_VARIABLE,
  OBJ_TYPE,
  OBJ_FUNCTION,
  OBJ_PROCEDURE,
  OBJ_PARAMETER,
  OBJ_PROGRAM
};

enum ParamKind {
  PARAM_VALUE,
  PARAM_REFERENCE
};

struct Type_ {
  enum TypeClass typeClass;
  int arraySize;
  struct Type_ *elementType;
};

typedef struct Type_ Type;
typedef struct Type_ BasicType;

// TODO:GK3
struct TypeNode_ {
  Type *type;
  struct TypeNode_ *next;
};

typedef struct TypeNode_ TypeNode;

// TODO:GK1
struct ConstantValue_ {
  enum TypeClass type;
  union {
    int intValue;
    char charValue;
    char stringValue[MAX_LENGTH];     // GK1: Lưu constantValue kiểu string
    double doubleValue;               // GK1: Lưu constantValue kiểu double
  };
};

typedef struct ConstantValue_ ConstantValue;

struct Scope_;
struct ObjectNode_;
struct Object_;

struct ConstantAttributes_ {
  ConstantValue* value;
};

struct VariableAttributes_ {
  Type *type;
  struct Scope_ *scope;
};

struct TypeAttributes_ {
  Type *actualType;
};

struct ProcedureAttributes_ {
  struct ObjectNode_ *paramList;
  struct Scope_* scope;
};

struct FunctionAttributes_ {
  struct ObjectNode_ *paramList;
  Type* returnType;
  struct Scope_ *scope;
};

struct ProgramAttributes_ {
  struct Scope_ *scope;
};

struct ParameterAttributes_ {
  enum ParamKind kind;
  Type* type;
  struct Object_ *function;
};

typedef struct ConstantAttributes_ ConstantAttributes;
typedef struct TypeAttributes_ TypeAttributes;
typedef struct VariableAttributes_ VariableAttributes;
typedef struct FunctionAttributes_ FunctionAttributes;
typedef struct ProcedureAttributes_ ProcedureAttributes;
typedef struct ProgramAttributes_ ProgramAttributes;
typedef struct ParameterAttributes_ ParameterAttributes;

struct Object_ {
  char name[MAX_IDENT_LEN];
  enum ObjectKind kind;
  union {
    ConstantAttributes* constAttrs;
    VariableAttributes* varAttrs;
    TypeAttributes* typeAttrs;
    FunctionAttributes* funcAttrs;
    ProcedureAttributes* procAttrs;
    ProgramAttributes* progAttrs;
    ParameterAttributes* paramAttrs;
  };
};

typedef struct Object_ Object;

struct ObjectNode_ {
  Object *object;
  struct ObjectNode_ *next;
};

typedef struct ObjectNode_ ObjectNode;

struct Scope_ {
  ObjectNode *objList;
  Object *owner;
  struct Scope_ *outer;
};

typedef struct Scope_ Scope;

struct SymTab_ {
  Object* program;
  Scope* currentScope;
  ObjectNode *globalObjectList;
};

typedef struct SymTab_ SymTab;

Type* makeIntType(void);
Type* makeCharType(void);
// TODO:GK1
Type* makeStringType(void);               // GK1: Bổ sung thêm 2 hàm mới vào file symtab.c
Type* makeDoubleType(void);               // GK1: Bổ sung thêm 2 hàm mới vào file symtab.c

Type* makeArrayType(int arraySize, Type* elementType);
Type* duplicateType(Type* type);
int compareType(Type* type1, Type* type2);

// TODO:GK3
int compareTypeInExpression(Type* type1, Type* type2);
int compareTypeInAssignment(Type* type1, Type* type2);

void freeType(Type* type);

ConstantValue* makeIntConstant(int i);
ConstantValue* makeCharConstant(char ch);
// TODO:GK1
ConstantValue* makeStringConstant(char str[MAX_LENGTH]);   // GK1: Bổ sung thêm hàm makeString
ConstantValue* makeDoubleConstant(double d);              // GK1: Bổ sung thêm hàm makeDouble

ConstantValue* duplicateConstantValue(ConstantValue* v);

Scope* createScope(Object* owner, Scope* outer);

Object* createProgramObject(char *programName);
Object* createConstantObject(char *name);
Object* createTypeObject(char *name);
Object* createVariableObject(char *name);
Object* createFunctionObject(char *name);
Object* createProcedureObject(char *name);
Object* createParameterObject(char *name, enum ParamKind kind, Object* owner);

Object* findObject(ObjectNode *objList, char *name);

void initSymTab(void);
void cleanSymTab(void);
void enterBlock(Scope* scope);
void exitBlock(void);
void declareObject(Object* obj);

// TODO:GK3
void addType(TypeNode** typeList, Type* type);
void freeTypeNode(TypeNode* typeList);
int countTypeNode(TypeNode* typeList);

#endif
