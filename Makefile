CC = gcc
CFLAGS = -Wall -Werror -Wextra -O2 -Iinclude

TARGET = vmm
OBJS = src/vmm.o src/kvm.o src/mem.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)