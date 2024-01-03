#pragma once

#include "AssignmentLHS.hpp"
#include "ExprNode.hpp"
#include "VariableScope.hpp"

class AssignmentExpr : public ExprNode
{
    VariableScope *scope;

  public:
    AssignmentLHS *lhs()
    {
        return dynamic_cast<AssignmentLHS *>(children[0].get());
    }

    ExprNode *rhs()
    {
        return dynamic_cast<ExprNode *>(children[1].get());
    }

    virtual const std::string node_type() const
    {
        return "ASSIGNMENT STMT";
    }

    virtual char expr_type()
    {
        return rhs()->expr_type();
    }

    AssignmentExpr(Parser *parser, VariableScope *scope,
                   std::unique_ptr<ASTnode> lhs, std::unique_ptr<ASTnode> rhs);
    virtual Value *codegen();
};
