; ModuleID = 'mini-c'
source_filename = "mini-c"

declare i32 @print_int(i32)

declare float @print_float(float)

define float @unary(i32 %n, float %m) {
entry:
  %sum = alloca float, align 4
  %result = alloca float, align 4
  %m2 = alloca float, align 4
  %n1 = alloca i32, align 4
  store i32 %n, i32* %n1, align 4
  store float %m, float* %m2, align 4
  store float 0.000000e+00, float* %sum, align 4
  %0 = load i32, i32* %n1, align 4
  %1 = load float, float* %m2, align 4
  %int2floattmp = sitofp i32 %0 to float
  %2 = fadd float %int2floattmp, %1
  store float %2, float* %result, align 4
  %3 = load float, float* %result, align 4
  %4 = call float @print_float(float %3)
  %5 = load float, float* %sum, align 4
  %6 = load float, float* %result, align 4
  %7 = fadd float %5, %6
  store float %7, float* %sum, align 4
  %8 = load i32, i32* %n1, align 4
  %9 = load float, float* %m2, align 4
  %10 = fneg float %9
  %int2floattmp3 = sitofp i32 %8 to float
  %11 = fadd float %int2floattmp3, %10
  store float %11, float* %result, align 4
  %12 = load float, float* %result, align 4
  %13 = call float @print_float(float %12)
  %14 = load float, float* %sum, align 4
  %15 = load float, float* %result, align 4
  %16 = fadd float %14, %15
  store float %16, float* %sum, align 4
  %17 = load i32, i32* %n1, align 4
  %18 = load float, float* %m2, align 4
  %19 = fneg float %18
  %20 = fneg float %19
  %int2floattmp4 = sitofp i32 %17 to float
  %21 = fadd float %int2floattmp4, %20
  store float %21, float* %result, align 4
  %22 = load float, float* %result, align 4
  %23 = call float @print_float(float %22)
  %24 = load float, float* %sum, align 4
  %25 = load float, float* %result, align 4
  %26 = fadd float %24, %25
  store float %26, float* %sum, align 4
  %27 = load i32, i32* %n1, align 4
  %28 = load float, float* %m2, align 4
  %29 = fneg float %28
  %int2floattmp5 = sitofp i32 %27 to float
  %30 = fadd float %int2floattmp5, %29
  %31 = fneg float %30
  store float %31, float* %result, align 4
  %32 = load float, float* %result, align 4
  %33 = call float @print_float(float %32)
  %34 = load float, float* %sum, align 4
  %35 = load float, float* %result, align 4
  %36 = fadd float %34, %35
  store float %36, float* %sum, align 4
  %37 = load float, float* %sum, align 4
  ret float %37
}
