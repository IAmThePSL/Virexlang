# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g -Iinclude

# Source files
SRCS = src/main.c src/lexer.c src/token.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = Virex

# Default target
all: $(TARGET)

# Declare phony targets
.PHONY: all clean rebuild

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Clean up object files and executable
clean:
	$(RM) $(OBJS) $(TARGET)

# Rebuild the project
rebuild: clean all
