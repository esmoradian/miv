CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
TARGET = miv
SRCDIR = src
OBJDIR = obj
DIRS = core input terminal buffer output events

# Include paths
INCLUDES = $(addprefix -I$(SRCDIR)/, $(DIRS))

# Source files
SRCS = $(shell find $(SRCDIR) -name '*.c')
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEPS = $(OBJS:.o=.d)

# Create object directories
OBJDIRS = $(addprefix $(OBJDIR)/, $(DIRS))

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(OBJDIRS)
	@touch $(OBJDIRS)/.keep

# Compile source files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@
	@echo >> $@

# Link the target
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

# Clean build files
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Debug build
debug: CFLAGS += -g
debug: all

# Run the editor
run: all
	./$(TARGET)

# Help target
help:
	@echo "Available targets:"
	@echo "  all    : Build the editor (default)"
	@echo "  clean  : Remove build files"
	@echo "  debug  : Build with debug symbols"
	@echo "  run    : Build and run the editor"
	@echo "  help   : Show this help message"

-include $(DEPS)

.PHONY: all clean debug run help directories
