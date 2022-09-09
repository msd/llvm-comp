#pragma once

#include "ASTnode.hpp"

class AssignmentLHS : public ASTnode
{
  public:
    string var_to;
    AssignmentLHS(string var_to) : var_to(var_to)
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
