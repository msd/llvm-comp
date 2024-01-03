#pragma once

#include "ASTnode.hpp"

class DeclNode : public ASTnode
{
  public:
    DeclNode(Parser *parser) : ASTnode(parser)
    {
    }
    virtual std::string node_type() const = 0;
};
