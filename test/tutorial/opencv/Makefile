CXX = g++
CXXFLAGS = -std=c++11 -Wall `pkg-config --cflags opencv4`
LDFLAGS = `pkg-config --libs opencv4`

SRC = captureHandler.cpp main.cpp
OBJ = $(SRC:.cpp=.o)
OUT = test_opencv.out

all: $(OUT)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	export LD_PRELOAD=/usr/lib/aarch64-linux-gnu/libcamera/v4l2-compat.so
	$(CXX) $(OBJ) -o $(OUT) $(LDFLAGS)

clean:
	rm -f $(OBJ) $(OUT)

.PHONY: all clean
