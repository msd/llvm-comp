#include "BlockNode.hpp"

#include "the_externs.hpp"

BlockNode::BlockNode(Parser *parser) : ASTnode(parser)
{
    _scope = VariableScope::inherit_vars(ActiveScopes.top());
    scope = _scope.get();
}

Value *BlockNode::codegen()
{
    local_decls()->codegen();
    stmt_list()->codegen();
    return Builder.getTrue();
}
