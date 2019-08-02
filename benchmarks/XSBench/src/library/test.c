#include "llama.h"
int main(void)
{
	int *x = calloc(sizeof(int), 1);
	free(x);
}
