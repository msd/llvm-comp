#pragma once

#include "ExprNode.hpp"

class ReturnValueNode : public ASTnode
{
  public:
    ExprNode *expr;
    ReturnValueNode(Parser *parser, std::unique_ptr<ASTnode> expr)
        : ASTnode(parser)
    {
        children.push_back(std::move(expr));

        this->expr = dynamic_cast<ExprNode *>(children[0].get());
    }

    virtual const std::string node_type() const
    {
        return "RETURN VALUE STMT";
    }

    virtual Value *codegen();
};
