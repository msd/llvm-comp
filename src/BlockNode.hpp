#pragma once

#include "ASTnode.hpp"
#include "VariableScope.hpp"

class BlockNode : public ASTnode
{
  private:
    unique_ptr<VariableScope> _scope;

  public:
    VariableScope *scope;

    ASTnode *local_decls()
    {
        return children[0].get();
    }

    ASTnode *stmt_list()
    {
        return children[1].get();
    }

    virtual const string node_type() const
    {
        return "BLOCK";
    }

    BlockNode(Parser *parser);
    virtual Value *codegen();
};
