#pragma once

#include "ASTnode.hpp"

class StmtListNode : public ASTnode
{
  public:
    StmtListNode(Parser *parser) : ASTnode(parser)
    {
    }
    virtual const std::string node_type() const
    {
        return "STMTS";
    }

    virtual Value *codegen();
};
