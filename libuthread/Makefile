# Target library
lib := libuthread.a

# Compiler and linker options
CC := gcc
AR := ar
CFLAGS := -Wall -Werror
ARFLAGS := rcs

# Source files
SRCS := queue.c testQ.c

# Object files
OBJS := $(SRCS:.c=.o)

# Executable program
PROG := test

# Default target
all: $(lib) $(PROG)

# Build the library archive
$(lib): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

# Compile source files into object files
$(OBJS): %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Build the executable program
$(PROG): $(OBJS) $(lib)
	$(CC) $(CFLAGS) $< -o $@ -L. -luthread

# Clean up
clean:
	rm -f $(OBJS) $(lib) $(PROG)
