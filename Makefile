# Compiler
CC := g++

# Directories
SRC_DIR := .
LIB_DIR := ./lib
INTF_DIR := $(LIB_DIR)/intf
OBJ_DIR := ./temp
BIN_DIR := ./bin

# Source files
SRC := $(wildcard $(SRC_DIR)/*.cpp)
INTF := $(wildcard $(INTF_DIR)/*.h)
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

# Compiler flags
CFLAGS := -I$(INTF_DIR)

# Linker flags
LDFLAGS := -L$(LIB_DIR)

# Output binary
TARGET := $(BIN_DIR)/main

# Default target
all: $(TARGET)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INTF)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files
$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ -o $@

# Clean up
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean