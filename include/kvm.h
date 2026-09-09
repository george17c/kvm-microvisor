#ifndef __KVM_H__
#define __KVM_H__

#include <linux/kvm.h>

struct vcpu {
    int fd;
};

struct vm {
    int fd;
    int mem_sz;
    void *mem_start;
    struct kvm_run *run;
    struct vcpu vcpu;
};

int kvm_init();
struct vm *kvm_create_vm(int kvm, __u64 mem_sz, __u32 slot);
int kvm_create_vcpu(int vmfd);
void kvm_destroy_vm(struct vm *vm);

#endif /* __KVM_H__ */
