#ifndef __VCPU_H__
#define __VCPU_H__

struct vm;

struct vcpu {
    struct kvm_run *run;
    size_t run_sz;
    int fd;
};

void vcpu_create(struct vm *vm);
void vcpu_run(struct vm *vm);

#endif /* __VCPU_H__ */
