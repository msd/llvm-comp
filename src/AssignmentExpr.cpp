#include "AssignmentExpr.hpp"
#include "my_errors.hpp"
#include "VariableScope.hpp"
#include "coercion.hpp"

extern IRBuilder<> Builder;

Value *AssignmentExpr::codegen()
{
	auto  var_to_name = lhs()->var_to;
	auto  AssignmentValue       = rhs()->codegen();
	Value *CastedValue;

	// auto var_decl = scope->getDecl(var_to_name);

	if (!scope->hasName(var_to_name))
	{
		throw semantic_error(string() + "Variable '" + var_to_name + "' is being assigned but never declared.");
	}


	switch (scope->getDecl(var_to_name)->var_type)
	{
	case INT_TYPE:
		CastedValue = convert_value_to_int(AssignmentValue);
		break;

	case FLOAT_TYPE:
		CastedValue = convert_value_to_float(AssignmentValue);
		break;

	case BOOL_TYPE:
		CastedValue = convert_value_to_bool(AssignmentValue);
		break;
	}

	auto Address = scope->getAddr(var_to_name);

	return Builder.CreateStore(CastedValue, Address);
}

AssignmentExpr::AssignmentExpr(VariableScope *scope, unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs)
	: scope(scope)
{
	children.push_back(move(lhs));
	children.push_back(move(rhs));
	if (this->rhs()->expr_type() == VOID_TYPE)
	{
		throw semantic_error(" void value not ignored as it ought to be"); // Same error as gcc
	}
}