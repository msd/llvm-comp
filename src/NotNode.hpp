#pragma once

#include "RvalNode.hpp"

class NotNode : public RvalNode
{
  public:
    NotNode(Parser *parser, unique_ptr<ASTnode> sub) : RvalNode(parser)
    {
        addSub(move(sub));
    }

    RvalNode *negatee()
    {
        return dynamic_cast<RvalNode *>(children[0].get());
    }

    virtual const string node_type() const
    {
        return "NOT";
    }

    virtual char expr_type();

    virtual Value *codegen();
};
