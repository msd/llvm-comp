#pragma once

#include "RvalNode.hpp"
#include "ValidTypes.hpp"

class NegationNode : public RvalNode
{
  public:
    NegationNode(Parser *parser, std::unique_ptr<ASTnode> rval)
        : RvalNode(parser)
    {
        addSub(std::move(rval));
    }

    RvalNode *negatee()
    {
        return dynamic_cast<RvalNode *>(children[0].get());
    }

    virtual const std::string node_type() const
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
