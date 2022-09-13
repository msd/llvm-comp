#pragma once

#include "LocalDeclsNode.hpp"
#include "StmtListNode.hpp"
#include "VariableScope.hpp"

class BlockNode : public ASTnode
{
  private:
    unique_ptr<VariableScope> _scope;

  public:
    VariableScope *scope();

    LocalDeclsNode *local_decls()
    {
        return dynamic_cast<LocalDeclsNode *>(children[0].get());
    }

    StmtListNode *stmt_list()
    {
        return dynamic_cast<StmtListNode *>(children[1].get());
    }

    virtual const string node_type() const
    {
        return "BLOCK";
    }

    BlockNode(Parser *parser, unique_ptr<VariableScope> scope,
              unique_ptr<LocalDeclsNode> decls, unique_ptr<StmtListNode> stmts);
    virtual Value *codegen();
};
