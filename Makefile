# Target executable name
TARGET := app

# Compiler and flags from config.h
# We extract them directly from the header file
CC := $(shell grep '^#define CCAN_COMPILER ' config.h | sed 's/.*"\(.*\)"/\1/')
CFLAGS := $(shell grep '^#define CCAN_CFLAGS ' config.h | sed 's/.*"\(.*\)"/\1/')

# Source files
# Main application source
APP_SRC := tal_sample.c
# CCAN module sources
# We need tal.c. NOTE: tal might depend on other ccan modules.
# If you get linking errors about undefined ccan_* symbols,
# add the corresponding .c files here (e.g., ccan/str/str.c).
CCAN_SRCS := \
./ccan/tal/tal.c \
./ccan/htable/htable.c \
./ccan/likely/likely.c \
./ccan/take/take.c \
./ccan/str/str.c \
./ccan/str/debug.c \
./ccan/hash/hash.c \
./ccan/list/list.c

# All source files
SRCS := $(APP_SRC) $(CCAN_SRCS)

# Object files (place them alongside source files)
OBJS := $(patsubst %.c, %.o, $(SRCS))

# Default target
all: $(TARGET)

# Link target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# Compile source files to object files
%.o: %.c config.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
