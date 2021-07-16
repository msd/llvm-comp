; ModuleID = 'mini-c'
source_filename = "mini-c"

declare float @print_float(float)

define float @cosine(float %x) {
entry:
  %alt = alloca float, align 4
  %eps = alloca float, align 4
  %term = alloca float, align 4
  %n = alloca float, align 4
  %cos = alloca float, align 4
  %x1 = alloca float, align 4
  store float %x, float* %x1, align 4
  store float 0x3EB0C6F7A0000000, float* %eps, align 4
  store float 1.000000e+00, float* %n, align 4
  store float 1.000000e+00, float* %cos, align 4
  store float 1.000000e+00, float* %term, align 4
  store float -1.000000e+00, float* %alt, align 4
  br label %whilecond

whilecond:                                        ; preds = %whilebody, %entry
  %0 = load float, float* %term, align 4
  %1 = load float, float* %eps, align 4
  %2 = fcmp ugt float %0, %1
  br i1 %2, label %whilebody, label %afterwhile

whilebody:                                        ; preds = %whilecond
  %3 = load float, float* %term, align 4
  %4 = load float, float* %x1, align 4
  %5 = fmul float %3, %4
  %6 = load float, float* %x1, align 4
  %7 = fmul float %5, %6
  %8 = load float, float* %n, align 4
  %9 = fdiv float %7, %8
  %10 = load float, float* %n, align 4
  %11 = fadd float %10, 1.000000e+00
  %12 = fdiv float %9, %11
  store float %12, float* %term, align 4
  %13 = load float, float* %cos, align 4
  %14 = load float, float* %alt, align 4
  %15 = load float, float* %term, align 4
  %16 = fmul float %14, %15
  %17 = fadd float %13, %16
  store float %17, float* %cos, align 4
  %18 = load float, float* %alt, align 4
  %19 = fneg float %18
  store float %19, float* %alt, align 4
  %20 = load float, float* %n, align 4
  %21 = fadd float %20, 2.000000e+00
  store float %21, float* %n, align 4
  br label %whilecond

afterwhile:                                       ; preds = %whilecond
  %22 = load float, float* %cos, align 4
  %23 = call float @print_float(float %22)
  %24 = load float, float* %cos, align 4
  ret float %24
}
