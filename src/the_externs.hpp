#pragma once
#include "llvm/IR/IRBuilder.h"
#include <stack>
#include <memory>
#include <map>
#include "Token.hpp"

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
extern TOKEN CurTok;
