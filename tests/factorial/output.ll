; ModuleID = 'mini-c'
source_filename = "mini-c"

define i32 @factorial(i32 %n) {
entry:
  %factorial = alloca i32, align 4
  %i = alloca i32, align 4
  %n1 = alloca i32, align 4
  store i32 %n, i32* %n1, align 4
  store i32 1, i32* %factorial, align 4
  store i32 1, i32* %i, align 4
  br label %whilecond

whilecond:                                        ; preds = %whilebody, %entry
  %0 = load i32, i32* %i, align 4
  %1 = load i32, i32* %n1, align 4
  %2 = icmp sle i32 %0, %1
  br i1 %2, label %whilebody, label %afterwhile

whilebody:                                        ; preds = %whilecond
  %3 = load i32, i32* %factorial, align 4
  %4 = load i32, i32* %i, align 4
  %5 = mul i32 %3, %4
  store i32 %5, i32* %factorial, align 4
  %6 = load i32, i32* %i, align 4
  %7 = add i32 %6, 1
  store i32 %7, i32* %i, align 4
  br label %whilecond

afterwhile:                                       ; preds = %whilecond
  %8 = load i32, i32* %factorial, align 4
  ret i32 %8
}
