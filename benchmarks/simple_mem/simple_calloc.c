#define SIZE_OF_MEM_WASTER 750000

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "library/llama.h"
#include <string.h>
#include <time.h>

void logo(void);
int main(void)
{
	ariel_enable();
	logo();
	int *var1 = malloc(sizeof(int)* SIZE_OF_MEM_WASTER);
	
	int i;

	/* Set each value of the array */
	for(i = 0; i < SIZE_OF_MEM_WASTER; i++)
	{
		var1[i] = i;
	//	printf("%d\n", var1[i]);
	}
	//printf("Done!\n");


	/*  Check each value of the array */
	char true = 1;
	for(i = 0; i < SIZE_OF_MEM_WASTER; i++)
	{
		if(var1[i] != i)
		{
			true = 0;
			break;
	       	}
	}
	if(true)
		printf("Successful!\n");
	else
		printf("Failed!\n");

	/*  Set one array equal to another */
	int *var2 = calloc(sizeof(int), SIZE_OF_MEM_WASTER);
	for(i = 0; i < SIZE_OF_MEM_WASTER; i++)
	{
		var2[i] = var1[i];
	}

	/*  Perform interleaving accesses */
	int l = 0;
	for(i = 0; i < SIZE_OF_MEM_WASTER; i++)
	{
		var1[i] = l;
		/*  Perform a bunch of 
		 *  intervening operations */
		int j;
		for(j = 0; j < 1; j++)
		{
			int k;
			for(k = 0; k < 5; k++)
			{
				l = j * k;
			}	
		}
		
	}
	printf("Done!\n");


	return 0;
}

void logo(void)
{

	printf(	"=========================\n Simple Memory Allocator\n\tCALLOC EDITION\n=========================\n");
}
