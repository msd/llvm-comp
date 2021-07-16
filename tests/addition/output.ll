; ModuleID = 'mini-c'
source_filename = "mini-c"

declare i32 @print_int(i32)

define i32 @addition(i32 %n, i32 %m) {
entry:
  %result = alloca i32, align 4
  %m2 = alloca i32, align 4
  %n1 = alloca i32, align 4
  store i32 %n, i32* %n1, align 4
  store i32 %m, i32* %m2, align 4
  %0 = load i32, i32* %n1, align 4
  %1 = load i32, i32* %m2, align 4
  %2 = add i32 %0, %1
  store i32 %2, i32* %result, align 4
  %3 = load i32, i32* %n1, align 4
  %4 = icmp eq i32 %3, 4
  %ifcond = icmp ne i1 %4, false
  br i1 %ifcond, label %then, label %else

then:                                             ; preds = %entry
  %5 = load i32, i32* %n1, align 4
  %6 = load i32, i32* %m2, align 4
  %7 = add i32 %5, %6
  %8 = call i32 @print_int(i32 %7)
  br label %ifcont

else:                                             ; preds = %entry
  %9 = load i32, i32* %n1, align 4
  %10 = load i32, i32* %m2, align 4
  %11 = mul i32 %9, %10
  %12 = call i32 @print_int(i32 %11)
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %13 = load i32, i32* %result, align 4
  ret i32 %13
}
