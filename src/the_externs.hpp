#pragma once

#include <map>
#include <memory>
#include <stack>

#include "llvm/IR/IRBuilder.h"

#include "FunctionSignature.hpp"
#include "Token.hpp"
#include "Tokenizer.hpp"

class VariableScope;
class FunDefNode;

using namespace llvm;

extern LLVMContext TheContext;
extern IRBuilder<> Builder;
extern std::unique_ptr<Module> TheModule;
extern std::stack<VariableScope *> ActiveScopes;
extern std::map<std::string, FunDefNode *> DefinedFunctions;
extern std::map<std::string, FunctionSignature *> ExternedFunctions;
