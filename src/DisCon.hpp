#pragma once

#include "RvalNode.hpp"

class DisCon : public RvalNode
{
  public:
    vector<unique_ptr<ASTnode>> clauses;
    void addClause(unique_ptr<ASTnode> clause)
    {
        clauses.push_back(move(clause));
    }

    virtual char expr_type();
};

class ConjunctionNode : public DisCon
{
  public:
    virtual const string node_type() const
    {
        return "CONJUNCTION";
    }

    virtual Value *codegen();
};

class DisjunctionNode : public DisCon
{
  public:
    virtual const string node_type() const
    {
        return "DISJUNCTION";
    }

    virtual Value *codegen();
};
