/* Copyright (C) 2018, Derrick Greenspan and 		*
 * the University of Central Florida 			*
 * Licensed under the terms of the MIT License. 	*
 * Please see the LICENSE file for more information 	*/

#include "llamalib.h"
#include "redefinitions.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

long long int timesCalled = 0;
int threshold = 0;
int size_ratio = SIZERATIO;

__attribute__((constructor))
  void llama_init() {
  char *env;

  env = getenv("LLAMA_THRESHOLD");
  if(env) {
    threshold = strtoul(env, NULL, 0);
  }
}

void *precalloc(void *ptr, size_t size, int level)
{
	void *return_ptr = mlm_malloc(size, level);
	return_ptr = memcpy(return_ptr, ptr, size);
	return return_ptr;
}

/********************************************************************************
 *  				Function redefinitions 				*
 ********************************************************************************/
int _fprintf(FILE *stream, const char *format)
{
	printf("!!Intercepted a fprintf!\n");
	return(fprintf(stream, format));
}

int _fputc(int character, FILE *stream)
{
	printf("!!Intercepted an fputc!\n");
	return (fputc(character, stream));
}

FILE *_fopen(const char *filename, const char *mode)
{

	printf("!!Intercepted an fopen!\n");
	return (fopen(filename, mode));
}

int _fclose(FILE *stream)
{
	printf("!!Intercepted an fclose!\n");
	return fclose(stream);
}

void * _internal_calloc(size_t nitems, size_t size)
{
	int level = _which_level(size * nitems, threshold);
	void *return_ptr = mlm_malloc(size*nitems, level);
	return_ptr = memset(return_ptr, 0, size*nitems);
	return return_ptr;

}

void * _internal__mm_malloc(size_t size, size_t align)
{
	return mlm_malloc(size, _which_level(size, threshold));
}

void *_internal_realloc(void *ptr, size_t new_size)
{
	return precalloc(ptr, new_size, _which_level(new_size, threshold));
}

void _internal_free(void *ptr)
{
	ariel_mlm_free(ptr);
}
/**************************************************************************************** 
 * 				End function redefinitions 				*
 ****************************************************************************************/


/****************************************************************************************
 * 				Internal Functions					*
 ****************************************************************************************/
size_t _score(size_t data_size)
{
	int localLLVMScore = LLVMScore;
	int score;

  score = data_size / localLLVMScore;

	return(score);
}
void *_internal_malloc(size_t size)
{
	return mlm_malloc(size,_which_level(size, threshold)); 
}

int _which_level(size_t data_size, size_t threshold)
{

		size_t score = _score(data_size);
	#if POOLBEHAVIOR == AUTOMATIC 
		int level;
		if(score >= threshold)
		{
			level = DRAMPOOL;
		}
		else
		{
			level = NVMPOOL;	
		}
    fprintf(stderr, "ID: %d\n  Score: %d\n  LLVM score: %d\n  Size: %d\n  Level: %d\n", ID, score, LLVMScore, data_size, level);

		return level;

	#elif POOLBEHAVIOR == DRAMPOOL
		return DRAMPOOL;

	#elif POOLBEHAVIOR == NVMPOOL
		return NVMPOOL;
       			
	#endif 

}

void _setSizeRatio(float thisSizeRatio)
{
	size_ratio = thisSizeRatio;
	return;
}

/****************************************************************************************
 * 				End Internal Functions					*
 ****************************************************************************************/

void setLLVMScore(int thisScore, int id)
{
	/*  Set the number of times this function will be called */
	LLVMScore = thisScore;
  ID = id;
	return;
}


