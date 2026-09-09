#include "kvm.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <stddef.h>
#include <err.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/kvm.h>

const uint8_t code[] = {
    0xba, 0xf8, 0x03, /* mov $0x3f8, %dx */
    0x00, 0xd8,       /* add %bl, %al */
    0x04, '0',        /* add $'0', %al */
    0xee,             /* out %al, (%dx) */
    0xb0, '\n',       /* mov $'\n', %al */
    0xee,             /* out %al, (%dx) */
    0xf4,             /* hlt */
};

int main(void)
{
    struct kvm_sregs sregs;
    struct vm *vm;
    int kvm;

    kvm = kvm_init();

    vm = kvm_create_vm(kvm, 0x1000, 0);

    memcpy(vm->mem_start, code, sizeof(code));

    ioctl(vm->vcpu.fd, KVM_GET_SREGS, &sregs);
    sregs.cs.base = 0;
    sregs.cs.selector = 0;
    ioctl(vm->vcpu.fd, KVM_SET_SREGS, &sregs);

    struct kvm_regs regs = {
        .rip = 0x0000,
        .rax = 2,
        .rbx = 2,
        .rflags = 0x2,
    };
    ioctl(vm->vcpu.fd, KVM_SET_REGS, &regs);

    while (1) {
        ioctl(vm->vcpu.fd, KVM_RUN, NULL);
        switch (vm->run->exit_reason) {
        case KVM_EXIT_HLT:
            puts("KVM_EXIT_HLT");
            goto done;
        case KVM_EXIT_IO:
            if (vm->run->io.direction == KVM_EXIT_IO_OUT &&
                vm->run->io.size == 1 &&
                vm->run->io.port == 0x3f8 &&
                vm->run->io.count == 1)
            putchar(*(((char *)vm->run) + vm->run->io.data_offset));
            else
            errx(1, "unhandled KVM_EXIT_IO");
            break;
        case KVM_EXIT_FAIL_ENTRY:
            errx(1, "KVM_EXIT_FAIL_ENTRY: hardware_entry_failure_reason = 0x%llx",
             (unsigned long long)vm->run->fail_entry.hardware_entry_failure_reason);
        case KVM_EXIT_INTERNAL_ERROR:
            errx(1, "KVM_EXIT_INTERNAL_ERROR: suberror = 0x%x",
                 vm->run->internal.suberror);
        }
    }

done:
    kvm_destroy_vm(vm);
    close(kvm);

    return 0;
}
