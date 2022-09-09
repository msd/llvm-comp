#include "IfWithElseNode.hpp"
#include "is_type.hpp"
#include "the_externs.hpp"

Value *IfWithElseNode::codegen()
{
    auto outer_function = Builder.GetInsertBlock()->getParent();
    auto cond_v = cond->codegen();
    Value *comparison;

    if (is_int(cond_v))
    {
        auto Z = ConstantInt::get(Type::getInt32Ty(TheContext), 0, true);
        comparison = Builder.CreateICmpNE(cond->codegen(), Z, "ifcond");
    }
    else if (is_float(cond_v))
    {
        auto Z = ConstantInt::get(Type::getInt1Ty(TheContext), 0, false);
        comparison = Builder.CreateFCmpUNE(cond->codegen(), Z, "ifcond");
    }
    else if (is_bool(cond_v))
    {
        auto Z = ConstantFP::get(Type::getFloatTy(TheContext), 0.0);
        comparison = Builder.CreateICmpNE(cond->codegen(), Z, "ifcond");
    }
    else
    {
        return nullptr;
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
    outer_function->getBasicBlockList().push_back(else_block);
    Builder.SetInsertPoint(else_block);
    else_body->codegen();
    Builder.CreateBr(merge_block);

    outer_function->getBasicBlockList().push_back(merge_block);
    Builder.SetInsertPoint(merge_block);
    return merge_block;
}
