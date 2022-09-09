#pragma once

#include <map>
#include <memory>
#include <stack>

#include "llvm/IR/IRBuilder.h"

#include "FunctionSignature.hpp"
#include "Token.hpp"

class VariableScope;
class FunDeclNode;

using std::map;
using std::stack;
using std::unique_ptr;
using namespace llvm;

extern LLVMContext TheContext;
extern IRBuilder<> Builder;
extern unique_ptr<Module> TheModule;
extern stack<VariableScope *> ActiveScopes;
extern map<string, FunDeclNode *> DefinedFunctions;
extern map<string, FunctionSignature *> ExternedFunctions;
extern TOKEN CurTok;
extern FILE *pFile;
