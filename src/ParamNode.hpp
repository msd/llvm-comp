#pragma once

#include "ASTnode.hpp"
#include "my_errors.hpp"

class ParamNode : public ASTnode
{
  public:
    const std::string name;
    const char type;
    ParamNode(Parser *parser, std::string name, const char type)
        : ASTnode(parser), name(name), type(type)
    {
    }
    virtual std::string node_type() const
    {
        return "PARAM";
    }

    virtual std::string to_string() const
    {
        return node_type() + " node NAME=" + name +
               ", TYPE=" + type_to_str(type);
    }

    Type *llvm_type();
    virtual Value *codegen();
};
