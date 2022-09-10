#include "Parser.hpp"

#include <memory>

#include "AssignmentExpr.hpp"
#include "AssignmentLHS.hpp"
#include "BlockNode.hpp"
#include "DisCon.hpp"
#include "ExternListNode.hpp"
#include "FunCallNode.hpp"
#include "FunctionSignature.hpp"
#include "IfStmt.hpp"
#include "IfWithElseNode.hpp"
#include "LocalDeclsNode.hpp"
#include "NullStmt.hpp"
#include "Ops.hpp"
#include "ReturnNothingNode.hpp"
#include "ReturnValueNode.hpp"
#include "StmtListNode.hpp"
#include "Tokenizer.hpp"
#include "VarExprNode.hpp"
#include "WhileStmt.hpp"
#include "the_externs.hpp"

using std::make_unique;
using std::move;

unique_ptr<ParenExprNode> parse_paren_expr()
{
    assert_tok(LPAR, "Expected opening parenthesis '('");
    tok->next(); // Consume open parenthesis '('
    auto r = parse_rval();

    assert_tok(RPAR, "Expected closing parenthesis ')' after rvalue");
    tok->next(); // Consume closing parenthesis ')'
    return make_unique<ParenExprNode>(move(r));
}

unique_ptr<NegationNode> parse_neg_term()
{
    assert_tok(MINUS, "Expected minus '-'");
    tok->next(); // Consume minus '-'
    return make_unique<NegationNode>(parse_rval());
}

unique_ptr<ExprNode> parse_expr()
{
    if (tok->CurTok.type == IDENT)
    {
        auto ident_tok = tok->CurTok;
        auto ident_name = tok->CurTok.lexeme;
        tok->next(); // Consume IDENT
        auto peek = tok->CurTok;
        if (tok->CurTok.type == ASSIGN)
        {
            tok->next(); // Consume '='
            auto lhs = make_unique<AssignmentLHS>(ident_name);
            return make_unique<AssignmentExpr>(ActiveScopes.top(), move(lhs),
                                               parse_expr());
        }
        else
        {
            // putting back the token doesn't actually restore any of the state
            // values :( assert(peek.type != IDENT); should be enforced to
            // preserve the IdentifierStr but since IdentifierStr is also equal
            // to the lexeme attribute it should be no problem. Also two
            // consecutive identifier tokens don't constitute a valid sentece of
            // this grammar hence should emit an error further down the
            // recursion anyway
            tok->put_back(peek);
            tok->CurTok = ident_tok;
            tok->IdentifierStr = ident_name;
            return parse_rval();
        }
    }
    else
    {
        return parse_rval();
    }
}

vector<unique_ptr<ASTnode>> parse_args()
{
    auto args = vector<unique_ptr<ASTnode>>();

    while (tok->CurTok.type != RPAR)
    {
        // todo check if current token is type
        args.push_back(parse_expr());
        if (tok->CurTok.type == COMMA)
        {
            tok->next(); // Consume ","
        }
    }
    return args;
}

unique_ptr<FloatNode> parse_rval_FLOAT_LIT()
{
    assert_tok(FLOAT_LIT, "Expected float literal");
    auto r = make_unique<FloatNode>(tok->FloatVal);

    tok->next(); // Consume FLOAT_LIT
    return r;
}

unique_ptr<IntNode> parse_rval_INT_LIT()
{
    assert_tok(INT_LIT, "Expected int literal");
    auto r = make_unique<IntNode>(tok->IntVal);

    tok->next(); // Consume INT_LIT
    return r;
}

unique_ptr<BoolNode> parse_rval_BOOL_LIT()
{
    assert_tok(BOOL_LIT, "Expected bool literal");
    auto r = make_unique<BoolNode>(tok->BoolVal);

    tok->next(); // Consume BOOL_LIT
    return r;
}

unique_ptr<RvalNode> parse_rval_var_or_fun()
{
    assert_tok(IDENT, "Expected identifier");
    auto ident_name = tok->CurTok.lexeme;

    tok->next();            // Consume IDENT
    if (tok->CurTok.type == LPAR) // Parse Function call
    {
        auto fun_call = make_unique<FunCallNode>(ident_name);
        assert_tok(LPAR, "This should not happen :(");
        tok->next(); // Consume opening paren "("
        fun_call->setSubs(parse_args());
        assert_tok(
            RPAR,
            "Expected closing right parenthesis ')' after function arguments");
        tok->next(); // Consume closing paren ')'
        return fun_call;
    }
    else // Parse Var
    {
        // Check variable is declared
        auto var_decl = ActiveScopes.top()->getDecl(ident_name);
        if (var_decl)
        {
            return make_unique<VarExprNode>(ActiveScopes.top(), ident_name,
                                            var_decl);
        }
        else // variable is not declared
        {
            throw semantic_error("Variable " + ident_name +
                                 " used but not declared");
        }
    }
}

unique_ptr<NotNode> parse_not_term()
{
    assert_tok(NOT, "Expected '!'");
    tok->next(); // Consume "!"
    return make_unique<NotNode>(parse_rval());
}

unique_ptr<ExprNode> parse_rval_term()
{
    switch (tok->CurTok.type)
    {
    case INT_LIT:
        return parse_rval_INT_LIT();

    case BOOL_LIT:
        return parse_rval_BOOL_LIT();

    case FLOAT_LIT:
        return parse_rval_FLOAT_LIT();

    case NOT:
        return parse_not_term();

    case MINUS:
        return parse_neg_term();

    case LPAR:
        return parse_paren_expr();

    default:
        return parse_rval_var_or_fun();
    }
}

unique_ptr<ExprNode> parse_rval_multiplication()
{
    unique_ptr<ExprNode> to_be_returned = parse_rval_term();
    bool loop = true;

    while (loop)
    {
        char l_type, r_type;
        unique_ptr<ASTnode> rhs;
        switch (tok->CurTok.type)
        {
        case ASTERISK:
            tok->next(); // consume "*"
            to_be_returned =
                make_unique<OpMULT>(move(to_be_returned), parse_rval_term());
            break;

        case DIV:
            tok->next(); // consume "/"
            to_be_returned =
                make_unique<OpDIV>(move(to_be_returned), parse_rval_term());
            break;

        case MOD:
            tok->next(); // consume "%"
            l_type =
                dynamic_cast<RvalNode *>(to_be_returned.get())->expr_type();
            r_type = dynamic_cast<RvalNode *>(rhs.get())->expr_type();
            rhs = parse_rval_term();
            if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
            {
                throw semantic_error("Invalid operands of '%' (have '" +
                                     type_to_str(l_type) + "' and '" +
                                     type_to_str(l_type) + "')");
            }
            to_be_returned =
                make_unique<OpMODULO>(move(to_be_returned), parse_rval_term());
            break;

        default:
            loop = false;
        }
    }
    return to_be_returned;
}

// todo refactor to make more sense (parses also subtraction)
unique_ptr<ExprNode> parse_rval_addition()
{
    auto to_be_returned = parse_rval_multiplication();
    bool loop = true;

    while (loop)
    {
        switch (tok->CurTok.type)
        {
        case PLUS:
            tok->next(); // consume "+"
            to_be_returned = make_unique<OpADD>(move(to_be_returned),
                                                parse_rval_multiplication());
            break;

        case MINUS:
            tok->next(); // consume "-"
            to_be_returned = make_unique<OpSUB>(move(to_be_returned),
                                                parse_rval_multiplication());
            break;

        default:
            loop = false;
        }
    }
    return to_be_returned;
}

unique_ptr<ExprNode> parse_rval_inequality()
{
    auto to_be_returned = parse_rval_addition();
    bool loop = true;

    while (loop)
    {
        switch (tok->CurTok.type)
        {
        case LT:
            tok->next(); // consume "<"
            to_be_returned =
                make_unique<OpLT>(move(to_be_returned), parse_rval_addition());
            break;

        case LE:
            tok->next(); // consume "<="
            to_be_returned =
                make_unique<OpLE>(move(to_be_returned), parse_rval_addition());
            break;

        case GE:
            tok->next(); // consume ">="
            to_be_returned =
                make_unique<OpGE>(move(to_be_returned), parse_rval_addition());
            break;

        case GT:
            tok->next(); // consume ">"
            to_be_returned =
                make_unique<OpGT>(move(to_be_returned), parse_rval_addition());
            break;

        default:
            loop = false;
        }
    }
    // cout << "No more rval ineqality " << CurTok.type << '\n';
    return to_be_returned;
}

unique_ptr<ExprNode> parse_rval_equality()
{
    auto to_be_returned = parse_rval_inequality();
    bool loop = true;

    while (loop)
    {
        switch (tok->CurTok.type)
        {
        case EQ:
            tok->next(); // consume "=="
            to_be_returned = make_unique<OpEQ>(move(to_be_returned),
                                               parse_rval_inequality());
            break;

        case NE:
            tok->next(); // consume "!="
            to_be_returned = make_unique<OpNE>(move(to_be_returned),
                                               parse_rval_inequality());
            break;

        default:
            loop = false;
        }
    }
    // cout << "No more rval equality " << CurTok.type << '\n';
    return to_be_returned;
}

unique_ptr<ExprNode> parse_rval_conjunction()
{
    auto equality = parse_rval_equality();

    if (tok->CurTok.type == AND)
    {
        auto to_be_returned = make_unique<ConjunctionNode>();
        to_be_returned->addSub(move(equality));
        while (tok->CurTok.type == AND)
        {
            tok->next(); // consume "&&"
            to_be_returned->addSub(parse_rval_equality());
        }
        return to_be_returned;
    }
    return equality;
    // cout << "No more rval conjunctions " << CurTok.type << '\n';
}

unique_ptr<ExprNode> parse_rval()
{
    auto con = parse_rval_conjunction();

    if (tok->CurTok.type == OR)
    {
        auto to_be_returned = make_unique<DisjunctionNode>();
        to_be_returned->addSub(move(con));
        while (tok->CurTok.type == OR)
        {
            tok->next(); // consume "||"
            to_be_returned->addSub(parse_rval_conjunction());
        }
        return to_be_returned;
    }

    // cout << "No more rval disunctions " << CurTok.type << '\n';
    return con;
}

unique_ptr<ParamNode> parse_param()
{
    int param_type = 0;

    switch (tok->CurTok.type)
    {
    case INT_TOK:
        param_type = INT_TYPE;
        break;

    case FLOAT_TOK:
        param_type = FLOAT_TYPE;
        break;

    case BOOL_TOK:
        param_type = BOOL_TYPE;
        break;

    default:
        throw syntax_error("Invalid param type");
    }
    tok->next(); // Consume type token
    assert_tok(IDENT, "Identifier must follow type in parameter list");
    auto r = make_unique<ParamNode>(tok->CurTok.lexeme, param_type);

    tok->next(); // Consume IDENT
    return r;
}

unique_ptr<ASTnode> parse_params()
{
    auto params = make_unique<ParamsNode>();

    if (tok->CurTok.type == VOID_TOK)
    {
        tok->next(); // Consume "void"
        assert_tok(RPAR, "Expected closing parenthesis after void keyword in "
                         "parameter list");
        return params;
    }
    if (tok->CurTok.type == RPAR)
    {
        return params;
    }
    params->addSub(parse_param());
    while (tok->CurTok.type != RPAR)
    {
        assert_tok(COMMA, "Expected comma after parameter in parameter list");
        tok->next(); // Consume ','
        params->addSub(parse_param());
    }
    return params;
}

unique_ptr<ASTnode> parse_extern()
{
    assert_tok(EXTERN, "Expected 'extern' keyword");
    tok->next(); // Consume "extern"
    char return_type;

    switch (tok->CurTok.type)
    {
    case INT_TOK:
        return_type = INT_TYPE;
        break;

    case FLOAT_TOK:
        return_type = FLOAT_TYPE;
        break;

    case BOOL_TOK:
        return_type = BOOL_TYPE;
        break;

    case VOID_TOK:
        return_type = VOID_TYPE;
        break;

    default:
        throw syntax_error("Type must follow extern keyword");
    }
    assert(tok->CurTok.type == INT_TOK || tok->CurTok.type == FLOAT_TOK ||
           tok->CurTok.type == BOOL_TOK || tok->CurTok.type == VOID_TOK);
    tok->next(); // Consume type token
    assert_tok(IDENT, "Expected function name directly after extern keyword");
    auto fun_name = tok->CurTok.lexeme;

    if (ExternedFunctions.count(fun_name))
    {
        throw semantic_error("Attempted to redefine extern function " +
                             fun_name);
    }
    tok->next(); // Consume IDENT
    assert_tok(LPAR,
               "Expected opening left parenthesis '(' after function name");
    tok->next(); // Consume "("
    auto fun_params = parse_params();

    assert_tok(RPAR,
               "Expected closing right parenthesis ')' after parameter list");
    tok->next(); // Consume ")"
    assert_tok(SC, "Expected semicolon after extern function signature");
    tok->next(); // Consume ";"
    auto tbr =
        make_unique<FunctionSignature>(fun_name, return_type, move(fun_params));

    return tbr;
}

unique_ptr<ExternListNode> parse_extern_list()
{
    auto extern_list = make_unique<ExternListNode>();

    while (tok->CurTok.type == EXTERN)
    {
        extern_list->addSub(parse_extern());
    }
    return extern_list;
}

unique_ptr<ASTnode> parse_local_decls()
{
    auto local_decls = make_unique<LocalDeclsNode>();
    bool loop = true;

    while (loop)
    {
        char var_type;
        switch (tok->CurTok.type)
        {
        case INT_TOK:
            var_type = INT_TYPE;
            break;

        case FLOAT_TOK:
            var_type = FLOAT_TYPE;
            break;

        case BOOL_TOK:
            var_type = BOOL_TYPE;
            break;

        default:
            loop = false;
        }
        if (loop)
        {
            tok->next(); // Consume type tok
            assert_tok(IDENT,
                       "Expected identifier after type in local declaration");

            auto var_name = tok->CurTok.lexeme;
            auto decl = make_unique<VarDeclNode>(ActiveScopes.top(), var_name,
                                                 var_type, false);
            ActiveScopes.top()->setDecl(var_name, decl.get());
            local_decls->addSub(move(decl));

            tok->next(); // Consume IDENT
            assert_tok(
                SC, "Expected semicolon ';' at the end of local declaration");
            tok->next(); // Consume ';'
        }
    }
    return local_decls;
}

unique_ptr<ASTnode> parse_expr_stmt()
{
    // todo check current token
    if (tok->CurTok.type == SC)
    {
        tok->next(); // Consume ';'
        return make_unique<NullStmt>();
    }

    auto expr_stmt = parse_expr();

    assert_tok(SC, "Expected semicolon ';' at the end of expression");
    tok->next(); // Consume ';'
    return expr_stmt;
}

unique_ptr<ASTnode> parse_block()
{
    auto new_block = make_unique<BlockNode>();

    ActiveScopes.push(new_block->scope);

    assert_tok(LBRA, "Expected opening left brace '{' at the start of block");
    tok->next(); // Consume "{"
    auto decls = parse_local_decls();
    auto stmts = parse_stmt_list();

    assert_tok(RBRA, "Expected closing right brace '}' at the start of block");
    tok->next(); // Consume "}"
    new_block->addSub(move(stmts));
    new_block->addSub(move(decls));
    ActiveScopes.pop();
    return new_block;
}

unique_ptr<ASTnode> parse_if_stmt()
{
    assert_tok(
        IF, "Expected if at start of if-statement. This should not happen :(");
    tok->next(); // Consume "if"
    assert_tok(LPAR, "Expected opening left parenthesis '(' after if keyword");
    tok->next(); // Consume '('
    auto if_cond = parse_expr();

    assert_tok(RPAR,
               "Expected closing right parenthesis ')' after if condition");
    tok->next(); // Consume ')'
    auto if_body = parse_block();

    if (tok->CurTok.type == ELSE)
    {
        tok->next(); // Consume "else"
        auto else_body = parse_block();
        return make_unique<IfWithElseNode>(move(if_cond), move(if_body),
                                           move(else_body));
    }
    else
    {
        return make_unique<IfStmt>(move(if_cond), move(if_body));
    }
}

unique_ptr<ASTnode> parse_while_stmt()
{
    assert_tok(WHILE, "Expected while keyword at the start of while loop. This "
                      "should not happen :(");
    tok->next(); // Consume "while"
    assert_tok(LPAR,
               "Expected opening left parenthesis '(' after while keyword");
    tok->next(); // Consume '('
    auto condition = parse_expr();

    assert_tok(RPAR,
               "Expected closing right parenthesis ')' after while condition");
    tok->next(); // Consume ')'
    auto loop_body = parse_stmt();

    return make_unique<WhileStmt>(move(condition), move(loop_body));
}

unique_ptr<ASTnode> parse_return_stmt()
{
    assert_tok(RETURN, "Expected return keyword at the start of return "
                       "statement. This should not happen :{");
    tok->next();          // Consume "return"
    if (tok->CurTok.type == SC) // Return nothing
    {
        tok->next(); // Consume ';'
        return make_unique<ReturnNothingNode>();
    }
    else // Return expr
    {
        auto stmt = make_unique<ReturnValueNode>(parse_expr());
        assert_tok(SC, "Expected semicolon ';' after return statement");
        tok->next(); // Consume ';'
        return stmt;
    }
}

unique_ptr<ASTnode> parse_stmt()
{
    switch (tok->CurTok.type)
    {
    case LBRA:
        return parse_block();

    case IF:
        return parse_if_stmt();

    case WHILE:
        return parse_while_stmt();

    case RETURN:
        return parse_return_stmt();

    default:
        return parse_expr_stmt();
    }
}

unique_ptr<ASTnode> parse_stmt_list()
{
    auto stmt_list = make_unique<StmtListNode>();

    while (tok->CurTok.type != RBRA)
    {
        stmt_list->addSub(parse_stmt());
    }
    return stmt_list;
}

// Parses global variable declarations and function definitions
unique_ptr<ASTnode> parse_decl()
{
    assert_tok_any({INT_TOK, FLOAT_TOK, BOOL_TOK, VOID_TOK},
                   "Expected type keyword at the start of declaration");
    if (tok->CurTok.type == VOID_TOK)
    {
        auto fun_type = VOID_TYPE;
        // Parse a void function
        tok->next(); // Consume "void"
        assert_tok(IDENT, "Expected identifier after void keyword");
        auto fun_name = tok->CurTok.lexeme;
        if (ActiveScopes.top()->getDecl(fun_name))
        {
            throw semantic_error("Name " + fun_name +
                                 " has already been declared a variable");
        }
        if (ExternedFunctions.count(fun_name))
        {
            throw semantic_error(
                "Function " + fun_name +
                " has already been defined as extern previously");
        }
        if (DefinedFunctions.count(fun_name))
        {
            throw semantic_error("Function " + fun_name +
                                 " has already been defined previously");
        }
        tok->next(); // Consume IDENT
        assert_tok(LPAR,
                   "Expected opening left parenthesis '(' after identifier");
        tok->next(); // Consume '('
        auto fun_params = parse_params();
        assert_tok(
            RPAR,
            "Expected closing right parenthesis ')' after parameter list");
        tok->next(); // Consume ')'
        auto fun_sig = make_unique<FunctionSignature>(fun_name, fun_type,
                                                      move(fun_params));
        auto fun_body = parse_block();
        auto fun_def = make_unique<FunDeclNode>(move(fun_sig), move(fun_body));
        DefinedFunctions[fun_name] = fun_def.get();
        return fun_def;
    }
    char decl_type;

    switch (tok->CurTok.type)
    {
    case INT_TOK:
        decl_type = INT_TYPE;
        break;

    case FLOAT_TOK:
        decl_type = FLOAT_TYPE;
        break;

    case BOOL_TOK:
        decl_type = BOOL_TYPE;
        // break;

        // default:
        // 	throw compiler_error();
    }
    tok->next(); // Consume type token
    auto decl_name = tok->CurTok.lexeme;

    if (ActiveScopes.top()->getDecl(decl_name) != nullptr)
    {
        throw semantic_error("Identifier '" + decl_name +
                             "' has been used before for variable declaration");
    }
    if (ExternedFunctions.count(decl_name))
    {
        throw semantic_error("Identifier '" + decl_name +
                             "' has been used before as extern");
    }
    if (DefinedFunctions.count(decl_name))
    {
        throw semantic_error("Identifier '" + decl_name +
                             "' has been used before as function definition");
    }

    tok->next(); // Consume IDENT
    if (tok->CurTok.type == SC)
    {
        // Parse variable decl
        tok->next(); // Consume ';'
        auto var_decl = make_unique<VarDeclNode>(ActiveScopes.top(), decl_name,
                                                 decl_type, true);
        ActiveScopes.top()->setDecl(decl_name, var_decl.get());
        return move(var_decl);
    }
    else if (tok->CurTok.type == LPAR)
    {
        // Parse function decl
        tok->next(); // Consume '('
        auto params = parse_params();
        assert_tok(
            RPAR,
            "Expected closing right parenthesis ')' after parameter list");
        tok->next(); // Consume ')'

        auto sig =
            make_unique<FunctionSignature>(decl_name, decl_type, move(params));
        auto body = parse_block();
        auto fun_def = make_unique<FunDeclNode>(move(sig), move(body));
        DefinedFunctions[decl_name] = fun_def.get();
        return fun_def;
    }
    else
    {
        throw syntax_error("Expected either semicolon or opening left "
                           "parenthesis after identifier");
    }
}

unique_ptr<DeclListNode> parse_decl_list()
{
    auto decl_list = make_unique<DeclListNode>();

    while (tok->CurTok.type != EOF_TOK)
    {
        decl_list->addSub(parse_decl());
    }
    assert(decl_list->children.size() > 0);
    // TODO error if no main
    return decl_list;
}

// program ::= extern_list decl_list | decl_list
unique_ptr<ProgramNode> parse_program()
{
    return make_unique<ProgramNode>(parse_extern_list(), parse_decl_list());
}

unique_ptr<ProgramNode> parser()
{
    return parse_program();
}
