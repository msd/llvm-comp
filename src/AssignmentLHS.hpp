#pragma once

#include "ASTnode.hpp"
#include "Parser.hpp"

class AssignmentLHS : public ASTnode
{
  public:
    std::string var_to;
    AssignmentLHS(Parser *parser, std::string var_to)
        : ASTnode(parser), var_to(var_to)
    {
    }
    virtual std::string node_type() const
    {
        return "ASSISGNENT LHS";
    }

    virtual std::string to_string() const
    {
        return node_type() + " node TARGET_VAR=" + var_to;
    }

    virtual Value *codegen();
};
