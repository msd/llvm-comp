#pragma once

#include <memory>

#include "BoolNode.hpp"
#include "DeclListNode.hpp"
#include "ExternListNode.hpp"
#include "FloatNode.hpp"
#include "FunDeclNode.hpp"
#include "IntNode.hpp"
#include "NegationNode.hpp"
#include "NotNode.hpp"
#include "ParamNode.hpp"
#include "ParenExprNode.hpp"
#include "ProgramNode.hpp"

unique_ptr<ParenExprNode> parse_paren_expr();
unique_ptr<NegationNode> parse_neg_term();
unique_ptr<ExprNode> parse_expr();
vector<unique_ptr<ASTnode>> parse_args();
unique_ptr<FloatNode> parse_rval_FLOAT_LIT();
unique_ptr<IntNode> parse_rval_INT_LIT();
unique_ptr<BoolNode> parse_rval_BOOL_LIT();
unique_ptr<RvalNode> parse_rval_var_or_fun();
unique_ptr<NotNode> parse_not_term();
unique_ptr<ExprNode> parse_rval_term();
unique_ptr<ExprNode> parse_rval_multiplication();
// todo refactor to make more sense (parses also subtraction)
unique_ptr<ExprNode> parse_rval_addition();
unique_ptr<ExprNode> parse_rval_inequality();
unique_ptr<ExprNode> parse_rval_equality();
unique_ptr<ExprNode> parse_rval_conjunction();
unique_ptr<ExprNode> parse_rval();
unique_ptr<ParamNode> parse_param();
unique_ptr<ASTnode> parse_params();
unique_ptr<ASTnode> parse_extern();
unique_ptr<ExternListNode> parse_extern_list();
unique_ptr<ASTnode> parse_local_decls();
unique_ptr<ASTnode> parse_expr_stmt();
unique_ptr<ASTnode> parse_block();
unique_ptr<ASTnode> parse_if_stmt();
unique_ptr<ASTnode> parse_while_stmt();
unique_ptr<ASTnode> parse_return_stmt();
unique_ptr<ASTnode> parse_stmt();
unique_ptr<ASTnode> parse_stmt_list();
// Parses global variable declarations and function definitions
unique_ptr<ASTnode> parse_decl();
unique_ptr<DeclListNode> parse_decl_list();
unique_ptr<ProgramNode> parse_program();
unique_ptr<ProgramNode> parser();
