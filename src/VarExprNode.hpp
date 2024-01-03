#pragma once

#include "RvalNode.hpp"

class VarDeclNode;
class VariableScope;

class VarExprNode : public RvalNode
{
  public:
    const std::string name;
    const VarDeclNode *decl;
    VariableScope *scope;
    VarExprNode(Parser *parser, VariableScope *scope, std::string name,
                VarDeclNode *decl)
        : RvalNode(parser), name(name), decl(decl), scope(scope)
    {
    }
    virtual std::string node_type() const
    {
        return "VAR EXPR";
    }

    virtual std::string to_string() const
    {
        return node_type() + " node NAME=" + name;
    }

    virtual char expr_type();

    virtual Value *codegen();
};
