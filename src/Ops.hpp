#pragma once

#include "OperatorNode.hpp"

class OpADD : public OperatorNode
{
  public:
    OpADD(Parser *parser, unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs)
        : OperatorNode(parser, move(lhs), move(rhs))
    {
    }
    virtual const string node_type() const
    {
        return "ADDITION OPERATOR";
    }

    virtual char expr_type();

    virtual Value *codegen();
};

class OpMULT : public OperatorNode
{
  public:
    OpMULT(Parser *parser, unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs)
        : OperatorNode(parser, move(lhs), move(rhs))
    {
    }
    virtual const string node_type() const
    {
        return "MULTIPLICATION OPERATOR";
    }

    virtual char expr_type();

    virtual Value *codegen();
};

class OpDIV : public OperatorNode
{
  public:
    OpDIV(Parser *parser, unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs)
        : OperatorNode(parser, move(lhs), move(rhs))
    {
    }
    virtual const string node_type() const
    {
        return "DIVISION OPERATOR";
    }

    virtual char expr_type();

    virtual Value *codegen();
};

class OpMODULO : public OperatorNode
{
  public:
    OpMODULO(Parser *parser, unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs)
        : OperatorNode(parser, move(lhs), move(rhs))
    {
    }
    virtual const string node_type() const
    {
        return "MODULO OPERATOR";
    }

    virtual char expr_type();

    virtual Value *codegen();
};

class OpSUB : public OperatorNode
{
  public:
    OpSUB(Parser *parser, unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs)
        : OperatorNode(parser, move(lhs), move(rhs))
    {
    }
    virtual const string node_type() const
    {
        return "SUBTRACTION OPERATOR";
    }

    virtual char expr_type();

    virtual Value *codegen();
};

class EqualityNode : public OperatorNode
{
  public:
    EqualityNode(Parser *parser, unique_ptr<ASTnode> lhs,
                 unique_ptr<ASTnode> rhs)
        : OperatorNode(parser, move(lhs), move(rhs))
    {
    }
    virtual const string node_type() const = 0;

    virtual char expr_type();
};

class InequalityNode : public OperatorNode
{
  public:
    InequalityNode(Parser *parser, unique_ptr<ASTnode> lhs,
                   unique_ptr<ASTnode> rhs)
        : OperatorNode(parser, move(lhs), move(rhs))
    {
    }
    virtual const string node_type() const = 0;

    virtual char expr_type();
};

class OpEQ : public EqualityNode
{
  public:
    OpEQ(Parser *parser, unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs)
        : EqualityNode(parser, move(lhs), move(rhs))
    {
    }
    virtual const string node_type() const
    {
        return "EQUALS OPERATOR";
    }

    virtual Value *codegen();
};

class OpNE : public EqualityNode
{
  public:
    OpNE(Parser *parser, unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs)
        : EqualityNode(parser, move(lhs), move(rhs))
    {
    }
    virtual const string node_type() const
    {
        return "NOT EQUALS OPERATOR";
    }

    virtual Value *codegen();
};

class OpLE : public InequalityNode
{
  public:
    OpLE(Parser *parser, unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs)
        : InequalityNode(parser, move(lhs), move(rhs))
    {
    }
    virtual const string node_type() const
    {
        return "LESS OR EQUAL OPERATOR";
    }

    virtual Value *codegen();
};

class OpLT : public InequalityNode
{
  public:
    OpLT(Parser *parser, unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs)
        : InequalityNode(parser, move(lhs), move(rhs))
    {
    }
    virtual const string node_type() const
    {
        return "LESS THAN OPERATOR";
    }

    virtual Value *codegen();
};

class OpGE : public InequalityNode
{
  public:
    OpGE(Parser *parser, unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs)
        : InequalityNode(parser, move(lhs), move(rhs))
    {
    }
    virtual const string node_type() const
    {
        return "GREATER OR EQUAL OPERATOR";
    }

    virtual Value *codegen();
};

class OpGT : public InequalityNode
{
  public:
    OpGT(Parser *parser, unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs)
        : InequalityNode(parser, move(lhs), move(rhs))
    {
    }
    virtual const string node_type() const
    {
        return "GREATER THAN OPERATOR";
    }

    virtual Value *codegen();
};
