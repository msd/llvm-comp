#include "coercion.hpp"

#include "comparisons.hpp"
#include "is_type.hpp"
#include "my_errors.hpp"
#include "the_externs.hpp"

Value *convert_float_to_int(Value *val)
{
    return Builder.CreateFPToSI(val, IntValues::type(), "convF2Itmp");
}

Value *convert_bool_to_int(Value *val)
{
    return Builder.CreateZExt(val, IntValues::type(), "convB2Itmp");
}

Value *convert_int_to_bool(Value *val)
{
    auto current_parent = Builder.GetInsertBlock()->getParent();
    auto cmp = create_true_cmp<int>(val, "convI2Bcmp");
    auto true_block =
        BasicBlock::Create(TheContext, "convI2true", current_parent);
    auto false_block =
        BasicBlock::Create(TheContext, "convI2Bfalse", current_parent);
    auto merge_block =
        BasicBlock::Create(TheContext, "convI2Bmerge", current_parent);

    Builder.CreateCondBr(cmp, true_block, false_block);

    Builder.SetInsertPoint(true_block);
    Builder.CreateBr(merge_block);

    Builder.SetInsertPoint(false_block);
    Builder.CreateBr(merge_block);

    Builder.SetInsertPoint(merge_block);
    auto phi = Builder.CreatePHI(BoolValues::type(), 2, "convI2Bresult");
    phi->addIncoming(BoolValues::True(), true_block);
    phi->addIncoming(BoolValues::False(), false_block);

    return phi;
}

Value *convert_float_to_bool(Value *val)
{
    auto current_parent = Builder.GetInsertBlock()->getParent();
    auto cmp = create_true_cmp<float>(val, "convF2Bcmp");
    auto true_block =
        BasicBlock::Create(TheContext, "convF2Btrue", current_parent);
    auto false_block =
        BasicBlock::Create(TheContext, "convF2Bfalse", current_parent);
    auto merge_block =
        BasicBlock::Create(TheContext, "convF2Bmerge", current_parent);

    Builder.CreateCondBr(cmp, true_block, false_block);

    Builder.SetInsertPoint(true_block);
    Builder.CreateBr(merge_block);

    Builder.SetInsertPoint(false_block);
    Builder.CreateBr(merge_block);

    Builder.SetInsertPoint(merge_block);
    auto phi = Builder.CreatePHI(BoolValues::type(), 2, "convF2Bresult");

    phi->addIncoming(BoolValues::True(), true_block);
    phi->addIncoming(BoolValues::False(), false_block);

    return phi;
}

Value *convert_int_to_float(Value *val)
{
    return Builder.CreateSIToFP(val, Type::getFloatTy(TheContext),
                                "int2floattmp");
}

Value *convert_bool_to_float(Value *val)
{
    auto current_parent = Builder.GetInsertBlock()->getParent();
    auto cmp = val;
    auto true_block =
        BasicBlock::Create(TheContext, "convB2Ftrue", current_parent);
    auto false_block =
        BasicBlock::Create(TheContext, "convB2Ffalse", current_parent);
    auto merge_block =
        BasicBlock::Create(TheContext, "convB2Fmerge", current_parent);

    Builder.CreateCondBr(cmp, true_block, false_block);

    Builder.SetInsertPoint(true_block);
    Builder.CreateBr(merge_block);

    Builder.SetInsertPoint(false_block);
    Builder.CreateBr(merge_block);

    Builder.SetInsertPoint(merge_block);
    auto phi = Builder.CreatePHI(FloatValues::type(), 2, "convB2Fresult");

    phi->addIncoming(FloatValues::One(), true_block);
    phi->addIncoming(FloatValues::Zero(), false_block);

    return phi;
}

Value *convert_value_to_int(Value *val)
{
    auto val_type = val->getType();

    if (is_int(val))
    {
        return val;
    }
    if (is_bool(val))
    {
        return convert_bool_to_int(val);
    }
    if (is_float(val))
    {
        return convert_float_to_int(val);
    }
    throw compiler_error("unknown type to convert to int type=" +
                         val->getType()->getStructName().str() + " " +
                         val->getType()->getTargetExtName().str());
}

Value *convert_value_to_float(Value *val)
{
    if (is_bool(val))
    {
        return convert_bool_to_float(val);
    }
    if (is_int(val))
    {
        return convert_int_to_float(val);
    }
    if (is_float(val))
    {
        return val;
    }
    throw compiler_error("unknown type to convert to float type=" +
                         val->getType()->getStructName().str() + " " +
                         val->getType()->getTargetExtName().str());
}

Value *convert_value_to_bool(Value *val)
{
    if (is_bool(val))
    {
        return val;
    }
    if (is_int(val))
    {
        return convert_int_to_bool(val);
    }
    if (is_float(val))
    {
        return convert_float_to_bool(val);
    }
    throw compiler_error("unknown type to convert to bool type=" +
                         val->getType()->getStructName().str() + " " +
                         val->getType()->getTargetExtName().str());
}
