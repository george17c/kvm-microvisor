#ifndef __MEM_H__
#define __MEM_H__

#include <stdint.h>

struct vm;

void vm_set_memory_region(struct vm *vm, uint64_t mem_sz, uint64_t entry);

#endif /* __MEM_H__ */
