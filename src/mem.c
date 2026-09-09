#include "mem.h"
#include "kvm.h"

#include <stdint.h>
#include <stddef.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

void vm_set_memory_region(struct vm *vm, __u64 mem_sz, __u32 slot)
{
    vm->mem_start = mmap(NULL, (size_t)mem_sz, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    vm->mem_sz = mem_sz;

    struct kvm_userspace_memory_region region = {
        .slot = slot,
        .guest_phys_addr = 0x0000,
        .memory_size = mem_sz,
        .userspace_addr = (uint64_t)vm->mem_start,
    };
    ioctl(vm->fd, KVM_SET_USER_MEMORY_REGION, &region);
}
