#pragma once

#include "RvalNode.hpp"

class OperatorNode : public RvalNode
{
  public:
    OperatorNode(Parser *parser, std::unique_ptr<ASTnode> lhs,
                 std::unique_ptr<ASTnode> rhs)
        : RvalNode(parser)
    {
        children.push_back(move(lhs));
        children.push_back(move(rhs));
    }

    RvalNode *lhs()
    {
        return dynamic_cast<RvalNode *>(children[0].get());
    }

    RvalNode *rhs()
    {
        return dynamic_cast<RvalNode *>(children[1].get());
    }

    virtual std::string to_string() const
    {
        return node_type() + " node with 2 children";
    }
};
