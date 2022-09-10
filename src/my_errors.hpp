#pragma once

#include "llvm/IR/IRBuilder.h"

#include "Token.hpp"
#include "ValidTypes.hpp"

using namespace llvm;
using std::exception;

class semantic_error : exception
{
    string err_msg;

  public:
    const int lineNo, columnNo;
    string erroneous_token;
    semantic_error(string err_msg);
    semantic_error(string err_msg, int lineNo, int columnNo,
                   string erroneous_token)
        : err_msg(move(err_msg)), lineNo(lineNo), columnNo(columnNo),
          erroneous_token(move(erroneous_token))
    {
    }
    virtual const char *what() const throw()
    {
        return err_msg.c_str();
    }
};

class syntax_error : exception
{
    string err_msg;

  public:
    const int lineNo, columnNo;
    string erroneous_token;
    syntax_error(string err_msg);
    syntax_error(string err_msg, int lineNo, int columnNo,
                 string erroneous_token)
        : err_msg(move(err_msg)), lineNo(lineNo), columnNo(columnNo),
          erroneous_token(move(erroneous_token))
    {
    }
    virtual const char *what() const throw()
    {
        return err_msg.c_str();
    }
};

class compiler_error : exception
// These should only happen if there's a change in the language specification.
// They are not something that will be emitted but here just for completeness
// sake. Happens when a token that wasn't expected is received.
{
    string err_msg;

  public:
    const int lineNo, columnNo;
    string erroneous_token;
    compiler_error();
    compiler_error(string err_msg);
    compiler_error(string err_msg, int lineNo, int columnNo,
                   string erroneous_token)
        : err_msg(move(err_msg)), lineNo(lineNo), columnNo(columnNo),
          erroneous_token(move(erroneous_token))
    {
    }
    virtual const char *what() const throw()
    {
        return err_msg.c_str();
    }
};

string type_to_str(const char type);

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
/// the function.  This is used for mutable variables etc.
// Copied from Kaleidoscope tutorial
// https://releases.llvm.org/10.0.0/docs/tutorial/MyFirstLanguageFrontend/LangImpl07.html
AllocaInst *CreateEntryBlockAlloca(Type *VarType, const string &VarName);
