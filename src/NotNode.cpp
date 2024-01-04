#include "NotNode.hpp"

#include "ValidTypes.hpp"
#include "coercion.hpp"
#include "comparisons.hpp"
#include "the_externs.hpp"

Value *NotNode::codegen()
{
    auto negatee_type = negatee()->expr_type();

    // if (negatee_type == BOOL_TYPE)
    // {
    //     auto TRUE = BoolValues::True();
    //     return Builder.CreateXor(negatee()->codegen(), TRUE, "nottemp");
    // }
    // else if (negatee_type == INT_TYPE)
    // {
    //     auto converted = convert_value_to_bool(negatee()->codegen());
    /*

    auto converted = convert_value_to_bool(negatee()->codegen());
    auto current_parent = Builder.GetInsertBlock()->getParent();
    auto cmp = int_cmp_zero(negatee()->codegen());

    auto then_block = BasicBlock::Create(TheContext, "then",
                                         current_parent); // boolean false
    auto else_block = BasicBlock::Create(TheContext, "else",
                                         current_parent); // boolean true
    auto merge_block =
        BasicBlock::Create(TheContext, "merge", current_parent);
    Builder.CreateCondBr(cmp, else_block, then_block);

    Builder.SetInsertPoint(then_block);
    auto then_v = BoolValues::False();
    Builder.CreateBr(merge_block);

    Builder.SetInsertPoint(else_block);
    auto else_v = BoolValues::True();
    Builder.CreateBr(merge_block);

    auto phi = Builder.CreatePHI(Type::getInt1Ty(TheContext), 2, "nottmp");
    phi->addIncoming(then_v, then_block);
    phi->addIncoming(else_v, else_block);
    return phi;
    */
    //     return Builder.CreateXor(converted, BoolValues::True(), "nottemp
    //     int");
    // }
    // else if (negatee_type == FLOAT_TYPE)
    // {
    //     auto Z = ConstantFP::get(Type::getFloatTy(TheContext), 0.0);
    //     auto comparison =
    //         Builder.CreateFCmpUEQ(negatee()->codegen(), Z, "zcmp");
    //     auto B1 = BasicBlock::Create(TheContext, "zcmptrue");
    //     auto B2 = BasicBlock::Create(TheContext, "zcmpfalse");
    //     return Builder.CreateCondBr(comparison, B1, B2);
    // auto convereted = convert_value_to_bool(negatee->codegen());
    //     return Builder.CreateXor(converted, BoolValues::True(), "nottemp
    //     int");
    // }
    // else
    // {
    //     return nullptr;
    // }

    auto TRUE = BoolValues::True();
    auto convereted = convert_value_to_bool(negatee()->codegen());
    return Builder.CreateXor(convereted, BoolValues::True(), "nottemp int");
}

char NotNode::expr_type()
{
    return BOOL_TYPE;
}
