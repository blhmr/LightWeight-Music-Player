# Makefile for raylib project

TARGET = main
CC = gcc

# Directories
SRCDIR = .
OBJDIR = obj
BINDIR = .

# Compiler flags
CFLAGS = -Wall -Wno-missing-braces -Iraylib/src

# Linker flags
LDFLAGS = -Lraylib/release/libs/linux -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# List of source files
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

# Build the project
all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

# Clean generated files
clean:
	rm -rf $(OBJDIR) $(BINDIR)/$(TARGET)

.PHONY: all clean
