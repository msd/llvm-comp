#pragma once

#include "RvalNode.hpp"

class NotNode : public RvalNode
{
  public:
    NotNode(Parser *parser, std::unique_ptr<ASTnode> sub) : RvalNode(parser)
    {
        addSub(std::move(sub));
    }

    RvalNode *negatee()
    {
        return dynamic_cast<RvalNode *>(children[0].get());
    }

    virtual std::string node_type() const
    {
        return "NOT";
    }

    virtual char expr_type();

    virtual Value *codegen();
};
