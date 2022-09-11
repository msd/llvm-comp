#pragma once

#include "ASTnode.hpp"
#include "BlockNode.hpp"
#include "ExprNode.hpp"

class IfWithElseNode : public ASTnode
{
  public:
    ExprNode *cond;
    BlockNode *then_body, *else_body;
    IfWithElseNode(Parser *parser, unique_ptr<ASTnode> if_cond,
                   unique_ptr<ASTnode> then_body, unique_ptr<ASTnode> else_body)
        : ASTnode(parser)
    {
        addSub(move(if_cond));
        addSub(move(then_body));
        addSub(move(else_body));
        cond = dynamic_cast<ExprNode *>(children[0].get());
        this->then_body = dynamic_cast<BlockNode *>(children[1].get());
        this->else_body = dynamic_cast<BlockNode *>(children[2].get());
    }

    virtual const string node_type() const
    {
        return "IF WITH ELSE STMT";
    }

    virtual Value *codegen();
};
