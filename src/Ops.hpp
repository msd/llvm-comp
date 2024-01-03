#pragma once

#include "OperatorNode.hpp"

class OpADD : public OperatorNode
{
  public:
    OpADD(Parser *parser, std::unique_ptr<ASTnode> lhs,
          std::unique_ptr<ASTnode> rhs)
        : OperatorNode(parser, std::move(lhs), std::move(rhs))
    {
    }
    virtual const std::string node_type() const
    {
        return "ADDITION OPERATOR";
    }

    virtual char expr_type();

    virtual Value *codegen();
};

class OpMULT : public OperatorNode
{
  public:
    OpMULT(Parser *parser, std::unique_ptr<ASTnode> lhs,
           std::unique_ptr<ASTnode> rhs)
        : OperatorNode(parser, std::move(lhs), std::move(rhs))
    {
    }
    virtual const std::string node_type() const
    {
        return "MULTIPLICATION OPERATOR";
    }

    virtual char expr_type();

    virtual Value *codegen();
};

class OpDIV : public OperatorNode
{
  public:
    OpDIV(Parser *parser, std::unique_ptr<ASTnode> lhs,
          std::unique_ptr<ASTnode> rhs)
        : OperatorNode(parser, std::move(lhs), std::move(rhs))
    {
    }
    virtual const std::string node_type() const
    {
        return "DIVISION OPERATOR";
    }

    virtual char expr_type();

    virtual Value *codegen();
};

class OpMODULO : public OperatorNode
{
  public:
    OpMODULO(Parser *parser, std::unique_ptr<ASTnode> lhs,
             std::unique_ptr<ASTnode> rhs)
        : OperatorNode(parser, std::move(lhs), std::move(rhs))
    {
    }
    virtual const std::string node_type() const
    {
        return "MODULO OPERATOR";
    }

    virtual char expr_type();

    virtual Value *codegen();
};

class OpSUB : public OperatorNode
{
  public:
    OpSUB(Parser *parser, std::unique_ptr<ASTnode> lhs,
          std::unique_ptr<ASTnode> rhs)
        : OperatorNode(parser, std::move(lhs), std::move(rhs))
    {
    }
    virtual const std::string node_type() const
    {
        return "SUBTRACTION OPERATOR";
    }

    virtual char expr_type();

    virtual Value *codegen();
};

class EqualityNode : public OperatorNode
{
  public:
    EqualityNode(Parser *parser, std::unique_ptr<ASTnode> lhs,
                 std::unique_ptr<ASTnode> rhs)
        : OperatorNode(parser, std::move(lhs), std::move(rhs))
    {
    }
    virtual const std::string node_type() const = 0;

    virtual char expr_type();
};

class InequalityNode : public OperatorNode
{
  public:
    InequalityNode(Parser *parser, std::unique_ptr<ASTnode> lhs,
                   std::unique_ptr<ASTnode> rhs)
        : OperatorNode(parser, std::move(lhs), std::move(rhs))
    {
    }
    virtual const std::string node_type() const = 0;

    virtual char expr_type();
};

class OpEQ : public EqualityNode
{
  public:
    OpEQ(Parser *parser, std::unique_ptr<ASTnode> lhs,
         std::unique_ptr<ASTnode> rhs)
        : EqualityNode(parser, std::move(lhs), std::move(rhs))
    {
    }
    virtual const std::string node_type() const
    {
        return "EQUALS OPERATOR";
    }

    virtual Value *codegen();
};

class OpNE : public EqualityNode
{
  public:
    OpNE(Parser *parser, std::unique_ptr<ASTnode> lhs,
         std::unique_ptr<ASTnode> rhs)
        : EqualityNode(parser, std::move(lhs), std::move(rhs))
    {
    }
    virtual const std::string node_type() const
    {
        return "NOT EQUALS OPERATOR";
    }

    virtual Value *codegen();
};

class OpLE : public InequalityNode
{
  public:
    OpLE(Parser *parser, std::unique_ptr<ASTnode> lhs,
         std::unique_ptr<ASTnode> rhs)
        : InequalityNode(parser, std::move(lhs), std::move(rhs))
    {
    }
    virtual const std::string node_type() const
    {
        return "LESS OR EQUAL OPERATOR";
    }

    virtual Value *codegen();
};

class OpLT : public InequalityNode
{
  public:
    OpLT(Parser *parser, std::unique_ptr<ASTnode> lhs,
         std::unique_ptr<ASTnode> rhs)
        : InequalityNode(parser, std::move(lhs), std::move(rhs))
    {
    }
    virtual const std::string node_type() const
    {
        return "LESS THAN OPERATOR";
    }

    virtual Value *codegen();
};

class OpGE : public InequalityNode
{
  public:
    OpGE(Parser *parser, std::unique_ptr<ASTnode> lhs,
         std::unique_ptr<ASTnode> rhs)
        : InequalityNode(parser, std::move(lhs), std::move(rhs))
    {
    }
    virtual const std::string node_type() const
    {
        return "GREATER OR EQUAL OPERATOR";
    }

    virtual Value *codegen();
};

class OpGT : public InequalityNode
{
  public:
    OpGT(Parser *parser, std::unique_ptr<ASTnode> lhs,
         std::unique_ptr<ASTnode> rhs)
        : InequalityNode(parser, std::move(lhs), std::move(rhs))
    {
    }
    virtual const std::string node_type() const
    {
        return "GREATER THAN OPERATOR";
    }

    virtual Value *codegen();
};
