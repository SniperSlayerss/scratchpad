CC       := clang
PKGS     := sdl3
CFLAGS   := -Wall -O2 $(shell pkg-config --cflags $(PKGS))
LDFLAGS  := $(shell pkg-config --libs $(PKGS))

# Folders
SRCDIR   := src
BUILDDIR := build

# The final target/executable
TARGET   := scratchpad

# Gather source files from SRCDIR
SRCS     := $(wildcard $(SRCDIR)/*.c)
# Transform .c in src/ to .o in build/
OBJS     := $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRCS))

.PHONY: all clean

all: $(TARGET)

# Link step: link all object files into the final binary
$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

# Compile step: compile .c files in src/ to .o files in build/
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)       # Create the build folder if it doesn't exist
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET) $(BUILDDIR)

