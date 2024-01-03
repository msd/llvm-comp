#pragma once

#include "ExprNode.hpp"

class WhileStmt : public ASTnode
{
  public:
    WhileStmt(Parser *parser, std::unique_ptr<ExprNode> condition,
              std::unique_ptr<ASTnode> loop_body)
        : ASTnode(parser)
    {
        children.push_back(std::move(condition));
        children.push_back(std::move(loop_body));
    }

    ExprNode *condition()
    {
        return static_cast<ExprNode *>(children[0].get());
    }

    ASTnode *loop_body()
    {
        return children[1].get();
    }

    virtual const std::string node_type() const
    {
        return "WHILE STMT";
    }

    virtual Value *codegen();
};
