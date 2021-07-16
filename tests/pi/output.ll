; ModuleID = 'mini-c'
source_filename = "mini-c"

define float @pi() {
entry:
  %i = alloca i32, align 4
  %PI = alloca float, align 4
  %flag = alloca i1, align 1
  store i1 true, i1* %flag, align 1
  store float 3.000000e+00, float* %PI, align 4
  store i32 2, i32* %i, align 4
  br label %whilecond

whilecond:                                        ; preds = %ifcont, %entry
  %0 = load i32, i32* %i, align 4
  %1 = icmp slt i32 %0, 100
  br i1 %1, label %whilebody, label %afterwhile

whilebody:                                        ; preds = %whilecond
  %2 = load i1, i1* %flag, align 1
  %ifcond = icmp ne i1 %2, false
  br i1 %ifcond, label %then, label %else

then:                                             ; preds = %whilebody
  %3 = load float, float* %PI, align 4
  %4 = load i32, i32* %i, align 4
  %5 = load i32, i32* %i, align 4
  %6 = add i32 %5, 1
  %7 = mul i32 %4, %6
  %8 = load i32, i32* %i, align 4
  %9 = add i32 %8, 2
  %10 = mul i32 %7, %9
  %int2floattmp = sitofp i32 %10 to float
  %11 = fdiv float 4.000000e+00, %int2floattmp
  %12 = fadd float %3, %11
  store float %12, float* %PI, align 4
  br label %ifcont

else:                                             ; preds = %whilebody
  %13 = load float, float* %PI, align 4
  %14 = load i32, i32* %i, align 4
  %15 = load i32, i32* %i, align 4
  %16 = add i32 %15, 1
  %17 = mul i32 %14, %16
  %18 = load i32, i32* %i, align 4
  %19 = add i32 %18, 2
  %20 = mul i32 %17, %19
  %int2floattmp1 = sitofp i32 %20 to float
  %21 = fdiv float 4.000000e+00, %int2floattmp1
  %22 = fsub float %13, %21
  store float %22, float* %PI, align 4
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %23 = load i1, i1* %flag, align 1
  %nottemp = xor i1 %23, true
  store i1 %nottemp, i1* %flag, align 1
  %24 = load i32, i32* %i, align 4
  %25 = add i32 %24, 2
  store i32 %25, i32* %i, align 4
  br label %whilecond

afterwhile:                                       ; preds = %whilecond
  %26 = load float, float* %PI, align 4
  ret float %26
}
