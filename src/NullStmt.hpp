#pragma once

#include "ASTnode.hpp"

class NullStmt : public ASTnode
{
  public:
    NullStmt(Parser *parser) : ASTnode(parser)
    {
    }
    virtual std::string node_type() const
    {
        return "NULL STMT";
    }

    virtual Value *codegen();
};
