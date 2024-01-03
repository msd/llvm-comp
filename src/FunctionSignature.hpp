#pragma once

#include "ParamsNode.hpp"

class FunctionSignature : public ASTnode
{
    std::vector<Type *> param_types();
    Type *return_type_llvm();

  public:
    std::string name;
    const char return_type;
    FunctionSignature(Parser *parser, std::string name, const char return_type,
                      std::unique_ptr<ASTnode> params);
    ParamsNode *params()
    {
        return dynamic_cast<ParamsNode *>(children[0].get());
    }

    virtual std::string node_type() const
    {
        return "FUNCTION SIGNATURE";
    }

    virtual std::string return_node_type()
    {
        return type_to_str(return_type);
    }

    virtual std::string to_string() const
    {
        return node_type() + " node RETURN=" + type_to_str(return_type);
    }

    virtual Function *codegen();
};
