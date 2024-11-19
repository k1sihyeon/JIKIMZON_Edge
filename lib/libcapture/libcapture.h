#ifndef LIB_CAPTURE_H
#define LIB_CAPTURE_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void externalFunction(int value);
void InitCapture(void** cap, int camIdx, int width, int height, int fps);
bool GetFrame(void* cap, uint8_t** buffer);

#ifdef __cplusplus
}
#endif

#endif // LIB_CAPTURE_H
