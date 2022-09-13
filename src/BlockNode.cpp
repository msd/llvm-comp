#include "BlockNode.hpp"

#include "the_externs.hpp"

BlockNode::BlockNode(Parser *parser, unique_ptr<VariableScope> scope,
                     unique_ptr<LocalDeclsNode> decls,
                     unique_ptr<StmtListNode> stmts)
    : ASTnode(parser)
{
    _scope = move(scope);
    children.push_back(move(decls));
    children.push_back(move(stmts));
}

VariableScope *BlockNode::scope()
{
    return _scope.get();
}

Value *BlockNode::codegen()
{
    local_decls()->codegen();
    stmt_list()->codegen();
    return Builder.getTrue();
}
