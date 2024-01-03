#pragma once

#include "ASTnode.hpp"
#include "BlockNode.hpp"
#include "ExprNode.hpp"

class IfStmt : public ASTnode
{
  public:
    ExprNode *cond;
    BlockNode *then_body;
    IfStmt(Parser *parser, std::unique_ptr<ASTnode> if_cond,
           std::unique_ptr<ASTnode> if_body)
        : ASTnode(parser)
    {
        addSub(std::move(if_cond));
        addSub(std::move(if_body));
        cond = dynamic_cast<ExprNode *>(children[0].get());
        then_body = dynamic_cast<BlockNode *>(children[1].get());
    }

    virtual const std::string node_type() const
    {
        return "IF STMT";
    }

    virtual Value *codegen();
};
