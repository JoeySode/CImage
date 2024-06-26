CFLAGS = -I./inc/ -Wall -Wextra -O3

SRC_DIR = src
BUILD_DIR = build

# Detect OS and architecture
UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

ifeq ($(UNAME_S),Linux)
    CC = gcc
    ifeq ($(UNAME_M),x86_64)
        CFLAGS += -m64
    endif
    ifeq ($(UNAME_M),arm64)
        CFLAGS += -march=armv8-a
    endif
endif
ifeq ($(UNAME_S),Darwin)
    CC = clang
endif
ifeq ($(OS),Windows_NT)
    LIB_NAME = libcimage.lib
    MKDIR = if not exist "$(1)" mkdir "$(1)"
    RM = if exist "$(1)" del /Q "$(1)"
    FIXPATH = $(subst /,\,$1)
else
    LIB_NAME = libcimage.a
    MKDIR = mkdir -p $(1)
    RM = rm -rf $(1)
    FIXPATH = $1
endif

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

LIBRARY = $(BUILD_DIR)/$(LIB_NAME)

all: $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	@$(call MKDIR,$(@D))
	ar rcs $@ $(OBJECTS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@$(call MKDIR,$(@D))
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(call RM,$(BUILD_DIR))

.PHONY: all clean
