# Compiler
CC = gcc

# Target
TARGET = component_tests

# Source files
SRCS = test_framework.c test_camera.c

# pkg-config command to get the necessary flags for gstreamer-1.0
PKG_CONFIG_FLAGS = $(shell pkg-config --cflags --libs gstreamer-1.0)

# Object files
OBJS = $(SRCS:.c=.o)

# Default rule
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(PKG_CONFIG_FLAGS)

# Compilation
%.o: %.c
	$(CC) -c $< $(PKG_CONFIG_FLAGS)

# Clean rule
clean:
	rm -f $(TARGET) $(OBJS)

# PHONY targets
.PHONY: all clean

