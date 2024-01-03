#include "AssignmentExpr.hpp"

#include "coercion.hpp"
#include "my_errors.hpp"
#include "the_externs.hpp"

Value *AssignmentExpr::codegen()
{
    auto var_to_name = lhs()->var_to;
    auto AssignmentValue = rhs()->codegen();
    Value *CastedValue;

    // auto var_decl = scope->getDecl(var_to_name);

    if (!scope->hasName(var_to_name))
    {
        using namespace std::string_literals;
        throw semantic_error("Variable '"s + var_to_name +
                                 "' is being assigned but never declared.",
                             &token);
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

AssignmentExpr::AssignmentExpr(Parser *parser, VariableScope *scope,
                               std::unique_ptr<ASTnode> lhs,
                               std::unique_ptr<ASTnode> rhs)
    : ExprNode(parser), scope(scope)
{
    children.push_back(std::move(lhs));
    children.push_back(std::move(rhs));
    if (this->rhs()->expr_type() == VOID_TYPE)
    {
        throw semantic_error(" void value not ignored as it ought to be",
                             &token); // Same error as gcc
    }
}
