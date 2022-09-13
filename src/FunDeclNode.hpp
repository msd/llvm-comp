#pragma once

#include "BlockNode.hpp"
#include "DeclNode.hpp"
#include "FunctionSignature.hpp"
#include "my_errors.hpp"

class FunDeclNode : public DeclNode
{
  public:
    FunDeclNode(Parser *parser, unique_ptr<FunctionSignature> sig,
                unique_ptr<BlockNode> body)
        : DeclNode(parser)
    {
        children.push_back(move(sig));
        children.push_back(move(body));
    }

    FunctionSignature *sig() const
    {
        return dynamic_cast<FunctionSignature *>(children[0].get());
    }

    BlockNode *body()
    {
        return dynamic_cast<BlockNode *>(children[1].get());
    }

    virtual const string node_type() const
    {
        return "FUN DECL";
    }

    virtual string to_string() const
    {
        return node_type() + " node NAME=" + sig()->name +
               " RETURN=" + type_to_str(sig()->return_type);
    }

    virtual Function *codegen();
};
