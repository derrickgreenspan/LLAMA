#include <stddef.h>
#include <stdint.h>

void *mlm_malloc(size_t size, int level);
void ariel_mlm_free(void *ptr);
void ariel_enable(void);
void ariel_malloc_flag(int mallocLocId, int count, int level);
