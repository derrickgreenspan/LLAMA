/* Copyright (C) 2018, Derrick Greenspan and 		*
 * the University of Central Florida 			*
 * Licensed under the terms of the MIT License. 	*
 * Please see the LICENSE file for more information 	*/

#define malloc(x) _internal_malloc(x)
//#define fputc(x, y) _fputc(x, y)
#define calloc(x, y) _internal_calloc(x, y)
//#define fopen(x, y) _fopen(x, y)
#define realloc(x, y) _internal_realloc(x, y)
#define free(x) _internal_free(x)
#define _mm_malloc(x, y) _internal__malloc(x, y)

#define SET_FAST _fast()
#define SET_PERSIST _persist()
