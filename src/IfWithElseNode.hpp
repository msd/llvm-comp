#pragma once

#include "ASTnode.hpp"
#include "BlockNode.hpp"
#include "ExprNode.hpp"

class IfWithElseNode : public ASTnode
{
  public:
    ExprNode *cond;
    BlockNode *then_body, *else_body;
    IfWithElseNode(Parser *parser, std::unique_ptr<ASTnode> if_cond,
                   std::unique_ptr<ASTnode> then_body,
                   std::unique_ptr<ASTnode> else_body)
        : ASTnode(parser)
    {
        addSub(std::move(if_cond));
        addSub(std::move(then_body));
        addSub(std::move(else_body));
        cond = dynamic_cast<ExprNode *>(children[0].get());
        this->then_body = dynamic_cast<BlockNode *>(children[1].get());
        this->else_body = dynamic_cast<BlockNode *>(children[2].get());
    }

    virtual std::string node_type() const
    {
        return "IF WITH ELSE STMT";
    }

    virtual Value *codegen();
};
