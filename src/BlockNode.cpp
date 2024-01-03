#include "BlockNode.hpp"

#include "the_externs.hpp"

BlockNode::BlockNode(Parser *parser, std::unique_ptr<VariableScope> scope,
                     std::unique_ptr<LocalDeclsNode> decls,
                     std::unique_ptr<StmtListNode> stmts)
    : ASTnode(parser)
{
    _scope = std::move(scope);
    children.push_back(std::move(decls));
    children.push_back(std::move(stmts));
}

Value *BlockNode::codegen()
{
    local_decls()->codegen();
    stmt_list()->codegen();
    return Builder.getTrue();
}
