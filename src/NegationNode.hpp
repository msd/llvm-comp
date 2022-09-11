#pragma once

#include "RvalNode.hpp"
#include "ValidTypes.hpp"

class NegationNode : public RvalNode
{
  public:
    NegationNode(Parser *parser, unique_ptr<ASTnode> rval)  : RvalNode(parser)
    {
        addSub(move(rval));
    }

    RvalNode *negatee()
    {
        return dynamic_cast<RvalNode *>(children[0].get());
    }

    virtual const string node_type() const
    {
        return "NEG";
    }

    virtual char expr_type()
    {
        auto negatee_type = negatee()->expr_type();

        if (negatee_type == BOOL_TYPE)
        {
            return INT_TYPE;
        }
        return negatee_type;
    }

    virtual Value *codegen();
};
