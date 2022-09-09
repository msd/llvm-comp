#include <iostream>
#include <cstdio>

// clang++ driver.cpp addition.ll -o add

#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

extern "C" DLLEXPORT int print_int(int X)
{
	fprintf(stderr, "%d\n", X);
	return 0;
}

extern "C" DLLEXPORT float print_float(float X)
{
	fprintf(stderr, "%f\n", X);
	return 0;
}

extern "C" {
	int addition(int, int);
}

using namespace std;

int main()
{
	int expected = 7;
	int got = addition(3, 4);
	if (expected == got)
	{
		cout << "[SUCCESS] addition " << endl;
	}
	else
	{
		cout << "[FAILURE] addition EXPECTED: " << expected << " GOT: " << got << endl;
	}
}
