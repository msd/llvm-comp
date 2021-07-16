// 1 * 2 + 3 * 4 * 5
extern void print_int(int x); 

void f()
{

}

int addition(int x)
{
    x = 10;
    return x;
}

int while_test(int y)
{
    int x;
    x = 0;
    while (x < 10) x = x + 1;
    return x;
}

int main()
{
    return while_test(10);
}

//clang -c -S -emit-llvm -O1 -g0 <sourcefile>