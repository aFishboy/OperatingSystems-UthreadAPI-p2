# Target library
lib := libuthread.a

# Default rule
all: $(lib)

# Avoid builtin rules and variables
MAKEFLAGS += -rR

# Don't print the commands unless explicitly requested with `make V=1`
ifneq ($(V),1)
Q = @
V = 0
endif

# Current directory
CUR_PWD := $(shell pwd)

# Define compilation toolchain
CC	= gcc

# General gcc options
CFLAGS	:= -Wall -Wextra -Werror
CFLAGS	+= -pipe
## Debug flag
ifneq ($(D),1)
CFLAGS	+= -O2
else
CFLAGS	+= -g
endif
## Include path
CFLAGS 	+= -I$(UTHREADPATH)
## Dependency generation
CFLAGS	+= -MMD

# static library compile
AR = ar
RCS = rcs

# Object files to compile
objs := queue.o uthread.o preempt.o context.o 
 
# Include dependencies
deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)

# Build the libuthread.a
$(lib): $(objs)
	@echo "CC	$@"
	$(Q)$(AR) $(RCS) $@ $^

# Generic rule for compiling objects
%.o: %.c
	@echo "CC	$@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

# Cleaning rule
clean: 
	@echo "CLEAN	$(CUR_PWD)"
	$(Q)rm -rf $(objs) $(deps) $(lib)
