#pragma once

#include "RvalNode.hpp"

class DisCon : public RvalNode
{
  public:
    explicit DisCon(Parser *parser) : RvalNode(parser)
    {
    }
    std::vector<std::unique_ptr<ASTnode>> clauses;
    void addClause(std::unique_ptr<ASTnode> clause)
    {
        clauses.push_back(std::move(clause));
    }

    virtual char expr_type();
};

class ConjunctionNode : public DisCon
{
  public:
    explicit ConjunctionNode(Parser *parser) : DisCon(parser)
    {
    }
    virtual std::string node_type() const
    {
        return "CONJUNCTION";
    }

    virtual Value *codegen();
};

class DisjunctionNode : public DisCon
{
  public:
    explicit DisjunctionNode(Parser *parser) : DisCon(parser)
    {
    }
    virtual std::string node_type() const
    {
        return "DISJUNCTION";
    }

    virtual Value *codegen();
};
