#include "WhileStmt.hpp"

#include "comparisons.hpp"
#include "the_externs.hpp"

Value *WhileStmt::codegen()
{
    auto outer_function = Builder.GetInsertBlock()->getParent();

    auto while_cond =
        BasicBlock::Create(TheContext, "whileCond", outer_function);
    auto while_loop =
        BasicBlock::Create(TheContext, "whileLoop", outer_function);
    auto after_while =
        BasicBlock::Create(TheContext, "endWhile", outer_function);

    Builder.SetInsertPoint(while_cond);
    auto comparison = create_expr_true_check(this->condition());
    Builder.CreateCondBr(comparison, while_loop, after_while);

    Builder.SetInsertPoint(while_loop);
    loop_body()->codegen();
    Builder.CreateBr(while_cond);

    Builder.SetInsertPoint(after_while);
    return Builder.getTrue();
}
