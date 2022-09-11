#pragma once

#include "ParamsNode.hpp"

class FunctionSignature : public ASTnode
{
  public:
    string name;
    const char return_type;
    FunctionSignature(Parser *parser, string name, const char return_type,
                      unique_ptr<ASTnode> params);
    ParamsNode *params()
    {
        return dynamic_cast<ParamsNode *>(children[0].get());
    }

    virtual const string node_type() const
    {
        return "FUNCTION SIGNATURE";
    }

    virtual string return_node_type()
    {
        return type_to_str(return_type);
    }

    virtual string to_string() const
    {
        return node_type() + " node RETURN=" + type_to_str(return_type);
    }

    virtual Function *codegen();
};
