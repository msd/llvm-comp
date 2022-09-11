#include "ASTnode.hpp"

#include "Parser.hpp"

ASTnode::ASTnode(Parser *parser) : token{*parser->current_token()}
{
}
