#pragma once

#include "BlockNode.hpp"
#include "DeclNode.hpp"
#include "FunctionSignature.hpp"
#include "my_errors.hpp"

class FunDefNode : public DeclNode
{
  public:
    FunDefNode(Parser *parser, std::unique_ptr<FunctionSignature> sig,
               std::unique_ptr<BlockNode> body)
        : DeclNode(parser)
    {
        children.push_back(std::move(sig));
        children.push_back(std::move(body));
    }

    FunctionSignature *sig() const
    {
        return dynamic_cast<FunctionSignature *>(children[0].get());
    }

    BlockNode *body()
    {
        return dynamic_cast<BlockNode *>(children[1].get());
    }

    virtual std::string node_type() const
    {
        return "FUN DECL";
    }

    virtual std::string to_string() const
    {
        return node_type() + " node NAME=" + sig()->name +
               " RETURN=" + type_to_str(sig()->return_type);
    }

    virtual Function *codegen();
};
