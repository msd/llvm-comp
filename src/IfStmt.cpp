#include "IfStmt.hpp"

#include "comparisons.hpp"
#include "is_type.hpp"
#include "the_externs.hpp"

Value *compare_condition(Value *cond_v)
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

Value *IfStmt::codegen()
{
    auto outer_function = Builder.GetInsertBlock()->getParent();
    auto cond_type = cond->expr_type();
    Value *cond_value = cond->codegen();
    Value *comparison = compare_condition(cond_value);
    auto then_block = BasicBlock::Create(TheContext, "then", outer_function);
    auto after_if = BasicBlock::Create(TheContext, "ifcont", outer_function);

    Builder.CreateCondBr(comparison, then_block, after_if);

    Builder.SetInsertPoint(then_block);
    then_body->codegen();
    Builder.CreateBr(after_if);

    outer_function->insert(outer_function->end(), after_if);
    // fixme remove
    // outer_function->getBasicBlockList().push_back(after_if);
    Builder.SetInsertPoint(after_if);
    return Builder.getTrue();
}
