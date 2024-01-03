#pragma once

#include "RvalNode.hpp"

class ParenExprNode : public RvalNode
{
  public:
    ParenExprNode(Parser *parser, std::unique_ptr<ASTnode> expr)
        : RvalNode(parser)
    {
        children.push_back(std::move(expr));
    }

    RvalNode *expr()
    {
        return dynamic_cast<RvalNode *>(children[0].get());
    }

    virtual const std::string node_type() const
    {
        return "PAREN EXPR";
    }

    virtual char expr_type()
    {
        return expr()->expr_type();
    }

    virtual Value *codegen();
};
