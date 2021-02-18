#include "trap.h"

int A[10];
int b;

int main()
{
	A[0] = 0;
	A[1] = 1;
	A[2] = 2;
	A[3] = 3;
	A[4] = 4;

	b = A[3];
	A[5] = b;

	nemu_assert(A[0] == 0);
	nemu_assert(A[1] >= 1);
	nemu_assert(A[2] <= 2);
	nemu_assert(A[3] > 2);
	nemu_assert(A[4] < 5);
	nemu_assert(b == 3);
	nemu_assert(A[5] != 6);

	HIT_GOOD_TRAP;

	return 0;
}
