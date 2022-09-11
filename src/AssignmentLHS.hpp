#pragma once

#include "ASTnode.hpp"
#include "Parser.hpp"

class AssignmentLHS : public ASTnode
{
  public:
    string var_to;
    AssignmentLHS(Parser *parser, string var_to)
        : ASTnode(parser), var_to(var_to)
    {
    }
    virtual const string node_type() const
    {
        return "ASSISGNENT LHS";
    }

    virtual string to_string() const
    {
        return node_type() + " node TARGET_VAR=" + var_to;
    }

    virtual Value *codegen();
};
