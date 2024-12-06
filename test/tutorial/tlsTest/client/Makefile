INCLUDES := -I.

CXX := g++
CXXFLAGS := -std=c++17 $(shell pkg-config --cflags openssl)
LDFLAGS = $(shell pkg-config --libs openssl)

SOURCES := client.cpp \
		   tls.cpp \
		   tcp.cpp
		   
TARGET := main.out

# 빌드 규칙
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS)