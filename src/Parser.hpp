#pragma once

#include <memory>
#include <vector>

#include "BlockNode.hpp"
#include "BoolNode.hpp"
#include "FloatNode.hpp"
#include "IntNode.hpp"
#include "NegationNode.hpp"
#include "NotNode.hpp"
#include "ParamNode.hpp"
#include "ParenExprNode.hpp"
#include "ProgramNode.hpp"
#include "TokenType.hpp"
#include "Tokenizer.hpp"

class Parser
{
    std::unique_ptr<ParenExprNode> parse_paren_expr();
    std::unique_ptr<NegationNode> parse_neg_term();
    std::unique_ptr<ExprNode> parse_expr();
    std::vector<std::unique_ptr<ASTnode>> parse_args();
    std::unique_ptr<FloatNode> parse_rval_FLOAT_LIT();
    std::unique_ptr<IntNode> parse_rval_INT_LIT();
    std::unique_ptr<BoolNode> parse_rval_BOOL_LIT();
    std::unique_ptr<RvalNode> parse_rval_var_or_fun();
    std::unique_ptr<NotNode> parse_not_term();
    std::unique_ptr<ExprNode> parse_rval_term();
    std::unique_ptr<ExprNode> parse_rval_multiplication();
    std::unique_ptr<ExprNode> parse_rval_addition();
    std::unique_ptr<ExprNode> parse_rval_inequality();
    std::unique_ptr<ExprNode> parse_rval_equality();
    std::unique_ptr<ExprNode> parse_rval_conjunction();
    std::unique_ptr<ExprNode> parse_rval();
    std::unique_ptr<ParamNode> parse_param();
    std::unique_ptr<ASTnode> parse_params();
    std::unique_ptr<ASTnode> parse_extern();
    std::unique_ptr<ExternListNode> parse_extern_list();
    std::unique_ptr<LocalDeclsNode> parse_local_decls();
    std::unique_ptr<ASTnode> parse_expr_stmt();
    std::unique_ptr<BlockNode> parse_block();
    std::unique_ptr<ASTnode> parse_if_stmt();
    std::unique_ptr<ASTnode> parse_while_stmt();
    std::unique_ptr<ASTnode> parse_return_stmt();
    std::unique_ptr<ASTnode> parse_stmt();
    std::unique_ptr<StmtListNode> parse_stmt_list();
    // Parses global variable declarations and function definitions
    std::unique_ptr<ASTnode> parse_decl();
    std::unique_ptr<DeclListNode> parse_decl_list();
    std::unique_ptr<TOKEN> CurTok;
    Tokenizer *tok;
    void assert_tok(TOKEN_TYPE tok_type, std::string err_msg);
    void assert_tok_any(std::vector<TOKEN_TYPE> tok_types, std::string err_msg);

  public:
    Parser(Tokenizer *tok);
    TOKEN *current_token();
    std::unique_ptr<ProgramNode> parse_program();
};
