/* Copyright (C) 2018, Derrick Greenspan and 		*
 * the University of Central Florida 			*
 * Licensed under the terms of the MIT License. 	*
 * Please see the LICENSE file for more information 	*/

#include <stddef.h>
#include <stdint.h>

void *mlm_malloc(size_t size, int level);
void ariel_mlm_free(void *ptr);
