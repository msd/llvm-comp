#pragma once

#include <stack>
#include <memory>
#include <map>

#include "llvm/IR/IRBuilder.h"

#include "Token.hpp"
#include "FunctionSignature.hpp"

class VariableScope;
class FunDeclNode;

using std::map;
using std::unique_ptr;
using std::stack;
using namespace llvm;

extern LLVMContext TheContext;
extern IRBuilder<> Builder;
extern unique_ptr<Module> TheModule;
extern stack<VariableScope *> ActiveScopes;
extern map<string, FunDeclNode *> DefinedFunctions;
extern map<string, FunctionSignature *> ExternedFunctions;
extern TOKEN CurTok;
extern FILE *pFile;
