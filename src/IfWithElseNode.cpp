#include "IfWithElseNode.hpp"

#include "comparisons.hpp"
#include "is_type.hpp"
#include "the_externs.hpp"

static auto compare_condition(Value *cond_v)
{
    if (is_int(cond_v))
    {
        return create_true_cmp<int>(cond_v, "ifcond_int");
    }
    if (is_bool(cond_v))
    {
        return create_true_cmp<bool>(cond_v, "ifcond_bool");
    }
    if (is_float(cond_v))
    {
        return create_true_cmp<float>(cond_v, "ifcond_float");
    }

    throw compiler_error("expression has unknown type");
}

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
