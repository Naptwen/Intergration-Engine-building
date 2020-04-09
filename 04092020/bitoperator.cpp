#include "bitoperator.h"
//size of char bit operator
void bitAdd(void* A, void* B,void* C, int byte)
{
	printf("%#010x\n", *(char*)(A));
	printf("%#010x\n", *(char*)(B));
	for (int i = 0; i < byte; i++)
	{
		*(char*)(&C + i) =
			  ((*((char*)A + i) & 128) & ((*((char*)B + i)) & 128))
			| ((*((char*)A + i) & 64) & ((*((char*)B + i)) & 64))
			| ((*((char*)A + i) & 32) & ((*((char*)B + i)) & 32))
			| ((*((char*)A + i) & 16) & ((*((char*)B + i)) & 16))
			| ((*((char*)A + i) & 8) & ((*((char*)B + i)) & 8))
			| ((*((char*)A + i) & 4) & ((*((char*)B + i)) & 4))
			| ((*((char*)A + i) & 2) & ((*((char*)B + i)) & 2))
			| ((*((char*)A + i) & 1) & ((*((char*)B + i)) & 1));
	}

}
//i-j = i+(~j+1)
const char bitSub(void* A, void* B)
{
	printf("%d", *(char*)A | (~(*(char*)B) + 1));
	return *(char*)A | (~(*(char*)B) + 1);
}