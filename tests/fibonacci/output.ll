; ModuleID = 'mini-c'
source_filename = "mini-c"

declare i32 @print_int(i32)

define i32 @fibonacci(i32 %n) {
entry:
  %total = alloca i32, align 4
  %c = alloca i32, align 4
  %next = alloca i32, align 4
  %second = alloca i32, align 4
  %first = alloca i32, align 4
  %n1 = alloca i32, align 4
  store i32 %n, i32* %n1, align 4
  %0 = load i32, i32* %n1, align 4
  %1 = call i32 @print_int(i32 %0)
  store i32 0, i32* %first, align 4
  store i32 1, i32* %second, align 4
  store i32 1, i32* %c, align 4
  store i32 0, i32* %total, align 4
  br label %whilecond

whilecond:                                        ; preds = %ifcont, %entry
  %2 = load i32, i32* %c, align 4
  %3 = load i32, i32* %n1, align 4
  %4 = icmp slt i32 %2, %3
  br i1 %4, label %whilebody, label %afterwhile

whilebody:                                        ; preds = %whilecond
  %5 = load i32, i32* %c, align 4
  %6 = icmp sle i32 %5, 1
  %ifcond = icmp ne i1 %6, false
  br i1 %ifcond, label %then, label %else

then:                                             ; preds = %whilebody
  %7 = load i32, i32* %c, align 4
  store i32 %7, i32* %next, align 4
  br label %ifcont

else:                                             ; preds = %whilebody
  %8 = load i32, i32* %first, align 4
  %9 = load i32, i32* %second, align 4
  %10 = add i32 %8, %9
  store i32 %10, i32* %next, align 4
  %11 = load i32, i32* %second, align 4
  store i32 %11, i32* %first, align 4
  %12 = load i32, i32* %next, align 4
  store i32 %12, i32* %second, align 4
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %13 = load i32, i32* %next, align 4
  %14 = call i32 @print_int(i32 %13)
  %15 = load i32, i32* %c, align 4
  %16 = add i32 %15, 1
  store i32 %16, i32* %c, align 4
  %17 = load i32, i32* %total, align 4
  %18 = load i32, i32* %next, align 4
  %19 = add i32 %17, %18
  store i32 %19, i32* %total, align 4
  br label %whilecond

afterwhile:                                       ; preds = %whilecond
  %20 = load i32, i32* %total, align 4
  %21 = call i32 @print_int(i32 %20)
  %22 = load i32, i32* %total, align 4
  ret i32 %22
}
