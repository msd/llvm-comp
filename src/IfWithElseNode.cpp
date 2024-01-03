#include "IfWithElseNode.hpp"

#include "is_type.hpp"
#include "the_externs.hpp"
#include <stdexcept>

Value *IfWithElseNode::codegen()
{
    auto outer_function = Builder.GetInsertBlock()->getParent();
    auto cond_v = cond->codegen();
    Value *comparison;

    if (is_int(cond_v))
    {
        comparison = create_true_cmp<int>(cond->codegen(), "ifcond_int");
    }
    else if (is_bool(cond_v))
    {
        comparison = create_true_cmp<bool>(cond->codegen(), "ifcond_bool");
    }
    else if (is_float(cond_v))
    {
        comparison = create_true_cmp<float>(cond->codegen(), "ifcond_float");
    }
    else
    {
        throw std::runtime_error("expression has unknown type");
    }
    auto then_block = BasicBlock::Create(TheContext, "then", outer_function);
    auto else_block = BasicBlock::Create(TheContext, "else");
    auto merge_block = BasicBlock::Create(TheContext, "ifcont");

    Builder.CreateCondBr(comparison, then_block, else_block);

    // Emit then body
    Builder.SetInsertPoint(then_block);
    then_body->codegen();
    Builder.CreateBr(merge_block);

    // Emit else body
    outer_function->insert(outer_function->end(), else_block);
    // fixme remove
    // outer_function->getBasicBlockList().push_back(else_block);
    Builder.SetInsertPoint(else_block);
    else_body->codegen();
    Builder.CreateBr(merge_block);

    outer_function->insert(outer_function->end(), merge_block);
    // fixme remove
    // outer_function->getBasicBlockList().push_back(merge_block);
    Builder.SetInsertPoint(merge_block);
    return merge_block;
}
