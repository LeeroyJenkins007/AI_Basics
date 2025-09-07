# ----------------------
# Compiler and flags
# ----------------------
CXX = g++
CXXFLAGS = -Wall -Iinclude

# ----------------------
# Directories
# ----------------------
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# ----------------------
# Source and object files
# ----------------------
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# ----------------------
# Target executable
# ----------------------
TARGET = $(BIN_DIR)/degrees

# ----------------------
# OS detection
# ----------------------
ifeq ($(OS),Windows_NT)
    # Windows settings
    MKDIR = if not exist $1 mkdir $1
    RM = if exist $1 rmdir /s /q $1
    ECHO = @echo
    COLOR =  # No ANSI colors on Windows
else
    # Linux/macOS settings
    MKDIR = mkdir -p $1
    RM = rm -rf $1
    ECHO = @echo
    COLOR_YELLOW = \033[1;33m
    COLOR_GREEN  = \033[0;32m
    COLOR_RESET  = \033[0m
    EMOJI_CHECK  = ✅
    EMOJI_HAMMER = 🔨
endif

# ----------------------
# Build rules
# ----------------------

all: $(TARGET)

# Link executable
$(TARGET): $(OBJS)
ifeq ($(OS),Windows_NT)
	$(ECHO) Linking...
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)
	$(ECHO) Build complete: $@
else
	$(ECHO) "$(EMOJI_HAMMER) $(COLOR_YELLOW)Linking...$(COLOR_RESET)"
	$(MKDIR,$(BIN_DIR))
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)
	$(ECHO) "$(EMOJI_CHECK) $(COLOR_GREEN)Build complete: $@$(COLOR_RESET)"
endif

# Compile object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
ifeq ($(OS),Windows_NT)
	$(ECHO) Compiling $<...
	if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	$(ECHO) Compiled $<
else
	$(ECHO) "$(EMOJI_HAMMER) $(COLOR_YELLOW)Compiling $<...$(COLOR_RESET)"
	$(MKDIR,$(BUILD_DIR))
	$(CXX) $(CXXFLAGS) -c $< -o $@
	$(ECHO) "$(EMOJI_CHECK) $(COLOR_GREEN)Compiled $<$(COLOR_RESET)"
endif

# Clean build artifacts
clean:
ifeq ($(OS),Windows_NT)
	if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)
	$(ECHO) Clean complete.
else
	$(RM,$(BUILD_DIR))
	$(RM,$(BIN_DIR))
	$(ECHO) Clean complete.
endif

.PHONY: all clean
