# Target library
lib := libuthread.a

# Compiler and linker options
CC := gcc
AR := ar
CFLAGS := -Wall -Werror
ARFLAGS := rcs

# Source files
SRCS := queue.c

# Object files
OBJS := $(SRCS:.c=.o)

# Default target
all: $(lib)

# Build the library archive
$(lib): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(lib)