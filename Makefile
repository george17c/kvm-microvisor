CC = gcc
CFLAGS = -Wall -Werror -Wextra -Iinclude -g

ARCH ?= $(shell uname -m)

TARGET = vmm
SRCS = src/vmm.c src/mem.c src/kvm.c src/arch/$(ARCH)/vcpu.c src/arch/$(ARCH)/test.c
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)