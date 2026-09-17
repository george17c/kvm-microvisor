#include "kvm.h"
#include "vcpu.h"

#include <string.h>

extern const uint8_t code[];
extern const int code_size;

int main(void)
{
    struct vm *vm;

    vm = kvm_create_vm(0x1000, 0x0000);
    vcpu_create(vm);

    memcpy(vm->mem_start, code, code_size);

    vcpu_run(vm);

    kvm_destroy_vm(vm);

    return 0;
}
