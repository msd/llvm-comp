#pragma once

#include "LocalDeclsNode.hpp"
#include "StmtListNode.hpp"
#include "VariableScope.hpp"

class BlockNode : public ASTnode
{
    std::unique_ptr<VariableScope> _scope;

  public:
    LocalDeclsNode *local_decls()
    {
        return dynamic_cast<LocalDeclsNode *>(children[0].get());
    }

    StmtListNode *stmt_list()
    {
        return dynamic_cast<StmtListNode *>(children[1].get());
    }

    virtual std::string node_type() const
    {
        return "BLOCK";
    }

    BlockNode(Parser *parser, std::unique_ptr<VariableScope> scope,
              std::unique_ptr<LocalDeclsNode> decls,
              std::unique_ptr<StmtListNode> stmts);
    virtual Value *codegen();
};
