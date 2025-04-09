# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -I. -lncurses -lpng
RELEASE_FLAGS = -O3
DEBUG_FLAGS = -g -O0

# Source files
SRC_DIR = .
SRC = $(wildcard $(SRC_DIR)/*.cpp) \
      $(wildcard lights/*.cpp) \
      $(wildcard objects/*.cpp) \
      $(wildcard shaders/*.cpp)

# Object files
OBJ_DIR = build
OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)
DEBUG_OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.debug.o)

# Executable names
TARGET = raytracer
DEBUG_TARGET = raytracer_debug

# Default target (release build)
all: $(TARGET)

# Release build
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(RELEASE_FLAGS) -o $@ $^

# Debug build
debug: $(DEBUG_TARGET)

$(DEBUG_TARGET): $(DEBUG_OBJ)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) -o $@ $^

# Compile source files into object files (release)
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(RELEASE_FLAGS) -c $< -o $@

# Compile source files into debug object files (debug)
$(OBJ_DIR)/%.debug.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(DEBUG_TARGET)

# Phony targets
.PHONY: all debug clean
