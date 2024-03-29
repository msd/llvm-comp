#pragma once

#include "ParamNode.hpp"

class ParamsNode : public ASTnode
{
  public:
    ParamsNode(Parser *parser) : ASTnode(parser)
    {
    }
    ParamNode *get(int i)
    {
        return dynamic_cast<ParamNode *>(children[i].get());
    }

    int param_count()
    {
        return children.size();
    }

    virtual std::string node_type() const
    {
        return "PARAMS";
    }

    virtual Value *codegen();
};
