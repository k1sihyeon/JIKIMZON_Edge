# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -I./inc `pkg-config --cflags --libs opencv4 libavcodec libavformat libavutil libswscale libcrypto openssl` -Wall -std=c++11

# Source and object files
SRC_DIR = src
OBJ_DIR = obj
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Executable name
TARGET = edge.out

# Default rule
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	export LD_PRELOAD=/usr/lib/aarch64-linux-gnu/libcamera/v4l2-compat.so
	$(CXX) -o $@ $^ $(CXXFLAGS)

# Compile each .cpp file into an .o file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Create the obj directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean up build files
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Phony targets
<<<<<<< HEAD
.PHONY: all clean
=======
.PHONY: all clean
>>>>>>> feature/tcp-opencv
