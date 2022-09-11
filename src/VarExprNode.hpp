#pragma once

#include "RvalNode.hpp"

class VarDeclNode;
class VariableScope;

class VarExprNode : public RvalNode
{
  public:
    const string name;
    const VarDeclNode *decl;
    VariableScope *scope;
    VarExprNode(Parser *parser, VariableScope *scope, string name,
                VarDeclNode *decl)
        : RvalNode(parser), name(name), decl(decl), scope(scope)
    {
    }
    virtual const string node_type() const
    {
        return "VAR EXPR";
    }

    virtual string to_string() const
    {
        return node_type() + " node NAME=" + name;
    }

    virtual char expr_type();

    virtual Value *codegen();
};
