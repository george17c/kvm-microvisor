#ifndef __MEM_H__
#define __MEM_H__

#include <linux/types.h>

struct vm;

void vm_set_memory_region(struct vm *vm, __u64 mem_sz, __u32 slot);

#endif /* __MEM_H__ */
