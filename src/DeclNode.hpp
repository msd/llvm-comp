#pragma once

#include "ASTnode.hpp"

class DeclNode : public ASTnode
{
  public:
    DeclNode(Parser *parser) : ASTnode(parser)
    {
    }
    virtual const string node_type() const = 0;
};
