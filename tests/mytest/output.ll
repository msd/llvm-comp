; ModuleID = 'mini-c'
source_filename = "mini-c"

declare void @print_int(i32)

define void @f() {
entry:
  ret void
}

define i32 @addition(i32 %x) {
entry:
  %x1 = alloca i32, align 4
  store i32 %x, i32* %x1, align 4
  store i32 10, i32* %x1, align 4
  %0 = load i32, i32* %x1, align 4
  ret i32 %0
}

define i32 @while_test(i32 %y) {
entry:
  %x = alloca i32, align 4
  %y1 = alloca i32, align 4
  store i32 %y, i32* %y1, align 4
  store i32 0, i32* %x, align 4
  br label %whilecond

whilecond:                                        ; preds = %whilebody, %entry
  %0 = load i32, i32* %x, align 4
  %1 = icmp slt i32 %0, 10
  br i1 %1, label %whilebody, label %afterwhile

whilebody:                                        ; preds = %whilecond
  %2 = load i32, i32* %x, align 4
  %3 = add i32 %2, 1
  store i32 %3, i32* %x, align 4
  br label %whilecond

afterwhile:                                       ; preds = %whilecond
  %4 = load i32, i32* %x, align 4
  ret i32 %4
}

define i32 @main() {
entry:
  %0 = call i32 @while_test(i32 10)
  ret i32 %0
}
