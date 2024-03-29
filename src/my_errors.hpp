#pragma once

#include "llvm/IR/IRBuilder.h"

#include "Token.hpp"
#include "ValidTypes.hpp"

using namespace llvm;

class semantic_error : std::exception
{
    std::string err_msg;

  public:
    const int lineNo, columnNo;
    std::string erroneous_token;
    semantic_error(std::string err_msg, TOKEN *token);
    semantic_error(std::string err_msg, int lineNo, int columnNo,
                   std::string erroneous_token)
        : err_msg(std::move(err_msg)), lineNo(lineNo), columnNo(columnNo),
          erroneous_token(std::move(erroneous_token))
    {
    }
    virtual const char *what() const throw()
    {
        return err_msg.c_str();
    }
};

class syntax_error : std::exception
{
    std::string err_msg;

  public:
    const int lineNo, columnNo;
    std::string erroneous_token;
    syntax_error(std::string err_msg, TOKEN *token);
    syntax_error(std::string err_msg, int lineNo, int columnNo,
                 std::string erroneous_token)
        : err_msg(std::move(err_msg)), lineNo(lineNo), columnNo(columnNo),
          erroneous_token(std::move(erroneous_token))
    {
    }
    virtual const char *what() const throw()
    {
        return err_msg.c_str();
    }
};

class compiler_error : std::exception
// These should only happen if there's a change in the language specification.
// They are not something that will be emitted but here just for completeness
// sake. Happens when a token that wasn't expected is received.
{
    std::string err_msg;

  public:
    const int lineNo, columnNo;
    bool noPos = false;
    std::string erroneous_token;
    compiler_error();
    compiler_error(std::string err_msg)
        : err_msg(err_msg), noPos(true), lineNo(0), columnNo(0)
    {
    }
    compiler_error(std::string err_msg, TOKEN *token);
    compiler_error(std::string err_msg, int lineNo, int columnNo,
                   std::string erroneous_token)
        : err_msg(move(err_msg)), lineNo(lineNo), columnNo(columnNo),
          erroneous_token(move(erroneous_token))
    {
    }
    virtual const char *what() const throw()
    {
        return err_msg.c_str();
    }
};

class random_error : std::runtime_error
{
};

std::string type_to_str(const char type);

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
/// the function.  This is used for mutable variables etc.
// Copied from Kaleidoscope tutorial
// https://releases.llvm.org/10.0.0/docs/tutorial/MyFirstLanguageFrontend/LangImpl07.html
AllocaInst *CreateEntryBlockAlloca(Type *VarType, const std::string &VarName);
