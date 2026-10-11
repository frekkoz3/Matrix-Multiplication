
CXX  := g++
NVCC := nvcc

TARGET := matmul
BUILD  := build

# C++ compiler flags
CXXFLAGS := -O3 -std=c++20 -Wall -Wextra

# CUDA compiler flags
NVCCFLAGS := -O3 -std=c++20 -arch=native

# Header files are under include/
CPPFLAGS := -Iinclude

# C++ source files
CPP_SRCS := \
	src/main.cpp \
	src/Arguments.cpp \
	src/Matrix.cpp \
	src/Timer.cpp \
	src/Utils.cpp \
	src/cpu/matrix_mul.cpp

# CUDA source file
CUDA_SRCS := src/cuda/matrix_mul.cu

# Generate object-file paths under build/
CPP_OBJS  := $(patsubst src/%.cpp,$(BUILD)/%.o,$(CPP_SRCS))
CUDA_OBJS := $(patsubst src/%.cu,$(BUILD)/%.o,$(CUDA_SRCS))
OBJS      := $(CPP_OBJS) $(CUDA_OBJS)

# Header dependency files
DEPS := $(OBJS:.o=.d)

.PHONY: all clean

all: $(TARGET)

# Link all object files
$(TARGET): $(OBJS)
	$(NVCC) $^ -o $@ -lcublas

# Compile C++ files
$(BUILD)/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -MF $(@:.o=.d) -c $< -o $@

# Compile CUDA files
$(BUILD)/%.o: src/%.cu
	mkdir -p $(dir $@)
	$(NVCC) $(CPPFLAGS) $(NVCCFLAGS) -MMD -MP -MF $(@:.o=.d) -c $< -o $@

# Include automatically generated header dependencies
-include $(DEPS)

clean:
	rm -rf $(BUILD) $(TARGET)
