#pragma once

#include <map>
#include <memory>
#include <stack>

#include "llvm/IR/IRBuilder.h"

#include "FunctionSignature.hpp"
#include "Token.hpp"
#include "Tokenizer.hpp"

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
extern FILE *pFile;
extern unique_ptr<Tokenizer> tok;
extern unique_ptr<TOKEN> CurTok;
