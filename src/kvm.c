#include "kvm.h"
#include "mem.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <stddef.h>
#include <err.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#define ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            fprintf(stderr, "%s:%d: ASSERTION FAILED: %s\n", \
                    __FILE__, __LINE__, msg); \
            abort(); \
        } \
    } while (0)

int kvm_init()
{
    int kvm, ret;

    kvm = open("/dev/kvm", O_RDWR | O_CLOEXEC);
    
    ret = ioctl(kvm, KVM_GET_API_VERSION, NULL);
        if (ret == -1)
                err(1, "KVM_GET_API_VERSION");
        if (ret != 12)
                errx(1, "KVM_GET_API_VERSION %d, expected 12", ret);

        ret = ioctl(kvm, KVM_CHECK_EXTENSION, KVM_CAP_USER_MEMORY);
        if (ret == -1)
                err(1, "KVM_CHECK_EXTENSION");
        if (!ret)
                errx(1, "Required extension KVM_CAP_USER_MEM not available");

    return kvm;
}

int kvm_create_vcpu(int vmfd)
{
    return ioctl(vmfd, KVM_CREATE_VCPU, (unsigned long)0);
}

struct vm *kvm_create_vm(int kvm, __u64 mem_sz, __u32 slot)
{
    struct vm *vm;
    struct vcpu vcpu;
    int mmap_size;

    vm = malloc(sizeof(struct vm));
    ASSERT(vm != NULL, "OOM: kvm_create_vm");

    vm->fd = ioctl(kvm, KVM_CREATE_VM, (unsigned long)0);

    vm_set_memory_region(vm, mem_sz, slot);

    vcpu.fd = kvm_create_vcpu(vm->fd);
    vm->vcpu = vcpu;

    mmap_size = ioctl(kvm, KVM_GET_VCPU_MMAP_SIZE, NULL);
    vm->run = mmap(NULL, mmap_size, PROT_READ | PROT_WRITE, MAP_SHARED, vm->vcpu.fd, 0);

    return vm;
}

void kvm_destroy_vm(struct vm *vm)
{
    free(vm);
}
