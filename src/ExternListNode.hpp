#pragma once

#include "ASTnode.hpp"

#include "ASTnode.hpp"

class ExternListNode : public ASTnode
{
  public:
    virtual const string node_type() const
    {
        return "EXTERN LIST";
    }

    virtual Value *codegen();
};
