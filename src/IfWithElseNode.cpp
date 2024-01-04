#include "IfWithElseNode.hpp"

#include "IfStmt.hpp"
#include "the_externs.hpp"

Value *IfWithElseNode::codegen()
{
    auto outer_function = Builder.GetInsertBlock()->getParent();
    Value *comparison = compare_condition(cond->codegen());

    auto then_block = BasicBlock::Create(TheContext, "then", outer_function);
    auto else_block = BasicBlock::Create(TheContext, "else", outer_function);
    auto merge_block = BasicBlock::Create(TheContext, "ifcont", outer_function);

    Builder.CreateCondBr(comparison, then_block, else_block);

    // Emit then body
    Builder.SetInsertPoint(then_block);
    then_body->codegen();
    Builder.CreateBr(merge_block);

    // Emit else body
    Builder.SetInsertPoint(else_block);
    else_body->codegen();
    Builder.CreateBr(merge_block);

    Builder.SetInsertPoint(merge_block);
    return merge_block;
}
