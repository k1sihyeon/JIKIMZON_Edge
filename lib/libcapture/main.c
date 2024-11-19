#include "libcapture.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main() {
    void* cap;
    int width = 1280;
    int height = 720;
    int fps = 30;

    InitCapture(&cap, 0, width, height, fps);

    printf("Capture device opened\n");

    uint8_t* buffer;

    while (!GetFrame(cap, &buffer)) {
        printf("Failed to capture frame\n");
    }

    printf("Frame captured\n");
    
    for (int i = 0; i < 10; i++) {
        printf("%d ", buffer[i]);
    }

    //externalFunction(42);

    return 0;
}

// g++ -c libcapture.cpp -o libcapture.o `pkg-config --cflags opencv4`
// g++ -shared -o libcapture.so libcapture.o `pkg-config --libs opencv4`
// gcc -o main main.c -L. -lcapture