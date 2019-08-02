#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#ifndef _LLADMALIB_H_
#define _LLADMALIB_H_

#define THRESHOLD 0
#define SIZERATIO 1

#define AUTOMATIC -1
#define DRAMPOOL 0
#define NVMPOOL 1
#define POOLBEHAVIOR AUTOMATIC
//#define POOLBEHAVIOR DRAMPOOL
//#define POOLBEHAVIOR AUTOMATIC
/*If DRAMPOOL, always write to DRAMPOOL, if NVMPOOL, always write to NVMPOOL */


/*  Counter for persistent variables - these counters are ultimately used 
 *  to label the files the persistent variables are mapped to */
int loop_counter;
long long int LLVMScore;

void pmfree(void *ptr, size_t size);
void setLLVMScore(int thisScore);
int _fprintf(FILE *stream, const char *format);
int _fputc(int character, FILE *stream);
void *_internal_malloc(size_t size);
void *_internal_calloc(size_t nitems, size_t size);
void *_internal_realloc(void *ptr, size_t new_size);
void * _internal__mm_malloc(size_t size, size_t align);
void _internal_free(void *ptr);
FILE *_fopen(const char *filename, const char *mode);
/*  Internal function _score */
size_t _score(size_t data_size);
int _which_level(size_t data_size, size_t threshold);

void reset_counter(void);

#endif
