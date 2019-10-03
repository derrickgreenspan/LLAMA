/* Copyright (C) 2018, Derrick Greenspan and 		*
 * the University of Central Florida 			*
 * Licensed under the terms of the MIT License. 	*
 * Please see the LICENSE file for more information 	*/

#include "redefinitions.h"
#include <numa.h>
#include <numaif.h>
#include <sys/mman.h>

void *mlm_malloc(size_t size, int level)
{
	void *ptr;
	nodemask_t nodemask;

  ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  if (ptr == MAP_FAILED) {
    perror("mmap2");
    exit(1);
  }

	nodemask_zero(&nodemask);
	nodemask_set_compat(&nodemask, level);

  if (mbind(ptr, size, MPOL_PREFERRED, &nodemask, sizeof(unsigned long), MPOL_MF_MOVE) < 0) {
    perror("mbind");
    exit(1);
  }

	return ptr;
}

void ariel_mlm_free(void *ptr)
{
}
