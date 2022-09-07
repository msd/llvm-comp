#pragma once

#include "OperatorNode.hpp"

class OpADD : public OperatorNode
{
public:
	OpADD(unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs)
		: OperatorNode(move(lhs), move(rhs)) {}
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
	OpMULT(unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs)
		: OperatorNode(move(lhs), move(rhs)) {}
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
	OpDIV(unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs)
		: OperatorNode(move(lhs), move(rhs)) {}
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
	OpMODULO(unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs)
		: OperatorNode(move(lhs), move(rhs)) {}
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
	OpSUB(unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs)
		: OperatorNode(move(lhs), move(rhs)) {}
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
	EqualityNode(unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs)
		: OperatorNode(move(lhs), move(rhs)) {}
	virtual const string node_type() const = 0;

	virtual char expr_type();
};

class InequalityNode : public OperatorNode
{
public:
	InequalityNode(unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs)
		: OperatorNode(move(lhs), move(rhs)) {}
	virtual const string node_type() const = 0;

	virtual char expr_type();
};

class OpEQ : public EqualityNode
{
public:
	OpEQ(unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs)
		: EqualityNode(move(lhs), move(rhs)) {}
	virtual const string node_type() const
	{
		return "EQUALS OPERATOR";
	}

	virtual Value *codegen();
};

class OpNE : public EqualityNode
{
public:
	OpNE(unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs)
		: EqualityNode(move(lhs), move(rhs)) {}
	virtual const string node_type() const
	{
		return "NOT EQUALS OPERATOR";
	}

	virtual Value *codegen();
};

class OpLE : public InequalityNode
{
public:
	OpLE(unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs)
		: InequalityNode(move(lhs), move(rhs)) {}
	virtual const string node_type() const
	{
		return "LESS OR EQUAL OPERATOR";
	}

	virtual Value *codegen();
};

class OpLT : public InequalityNode
{
public:
	OpLT(unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs)
		: InequalityNode(move(lhs), move(rhs)) {}
	virtual const string node_type() const
	{
		return "LESS THAN OPERATOR";
	}

	virtual Value *codegen();
};

class OpGE : public InequalityNode
{
public:
	OpGE(unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs)
		: InequalityNode(move(lhs), move(rhs)) {}
	virtual const string node_type() const
	{
		return "GREATER OR EQUAL OPERATOR";
	}

	virtual Value *codegen();
};

class OpGT : public InequalityNode
{
public:
	OpGT(unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs)
		: InequalityNode(move(lhs), move(rhs)) {}
	virtual const string node_type() const
	{
		return "GREATER THAN OPERATOR";
	}

	virtual Value *codegen();
};
