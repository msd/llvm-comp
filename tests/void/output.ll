; ModuleID = 'mini-c'
source_filename = "mini-c"

declare i32 @print_int(i32)

define void @Void() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %0 = load i32, i32* %result, align 4
  %1 = call i32 @print_int(i32 %0)
  br label %whilecond

whilecond:                                        ; preds = %whilebody, %entry
  %2 = load i32, i32* %result, align 4
  %3 = icmp slt i32 %2, 10
  br i1 %3, label %whilebody, label %afterwhile

whilebody:                                        ; preds = %whilecond
  %4 = load i32, i32* %result, align 4
  %5 = add i32 %4, 1
  store i32 %5, i32* %result, align 4
  %6 = load i32, i32* %result, align 4
  %7 = call i32 @print_int(i32 %6)
  br label %whilecond

afterwhile:                                       ; preds = %whilecond
  ret void
}
