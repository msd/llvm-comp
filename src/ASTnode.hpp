#pragma once

#include <memory>
#include <string>
#include <vector>

#include "llvm/IR/IRBuilder.h"

using namespace llvm;

using std::string;
using std::unique_ptr;
using std::vector;

/// ASTnode - Base class for all AST nodes.
class ASTnode
{
  public:
    vector<unique_ptr<ASTnode>> children{};
    virtual Value *codegen() = 0;
    virtual const string node_type() const = 0;

    virtual string to_string() const
    {
        return node_type() + " node with " + std::to_string(children.size()) +
               " child(ren)";
    }

    virtual ~ASTnode()
    {
    }
    virtual void addSub(unique_ptr<ASTnode> subExpr)
    {
        children.push_back(move(subExpr));
    }

    void setSubs(vector<unique_ptr<ASTnode>> subs)
    {
        children = move(subs);
    }

    ASTnode *getSub(int i)
    {
        return children[i].get();
    }
};
