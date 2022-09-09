#pragma once

#include "ASTnode.hpp"
#include "my_errors.hpp"

class ParamNode : public ASTnode
{
  public:
    const string name;
    const char type;
    ParamNode(string name, const char type) : name(name), type(type)
    {
    }
    virtual const string node_type() const
    {
        return "PARAM";
    }

    virtual string to_string() const
    {
        return node_type() + " node NAME=" + name +
               ", TYPE=" + type_to_str(type);
    }

    Type *llvm_type();
    virtual Value *codegen();
};
