#pragma once

#include <memory>
#include <string>
#include <vector>

#include "llvm/IR/IRBuilder.h"

#include "Token.hpp"

using namespace llvm;

class Parser;

/// ASTnode - Base class for all AST nodes.
class ASTnode
{
  public:
    TOKEN token;
    ASTnode(Parser *parser);
    std::vector<std::unique_ptr<ASTnode>> children{};
    virtual Value *codegen() = 0;
    virtual const std::string node_type() const = 0;

    virtual std::string to_string() const
    {
        return node_type() + " node with " + std::to_string(children.size()) +
               " child(ren)";
    }

    virtual ~ASTnode()
    {
    }

    virtual void addSub(std::unique_ptr<ASTnode> subExpr)
    {
        children.push_back(std::move(subExpr));
    }

    void setSubs(std::vector<std::unique_ptr<ASTnode>> subs)
    {
        children = move(subs);
    }

    ASTnode *getSub(int i)
    {
        return children[i].get();
    }
};
