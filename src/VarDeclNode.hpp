#pragma once

#include "DeclNode.hpp"
#include "ValidTypes.hpp"
#include "my_errors.hpp"

class VariableScope;

class VarDeclNode : public DeclNode
{
  public:
    VariableScope *scope;
    const std::string var_name;
    const char var_type;
    const bool is_global;
    VarDeclNode(Parser *parser, VariableScope *scope, std::string var_name,
                const char var_type, bool is_global)
        : DeclNode(parser), scope(scope), var_name(var_name),
          var_type(var_type), is_global(is_global)
    {
    }

    virtual const std::string node_type() const
    {
        return "VAR DECL";
    }

    virtual std::string to_string() const
    {
        return node_type() + " node NAME=" + var_name +
               " TYPE=" + type_to_str(var_type);
    }

    virtual Value *codegen();
};
