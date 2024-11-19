#include "captureHandler.hpp"

int main()
{
    CaptureHandler::GetInstance().StartCapture();
}

// opencv 빌드, LD_PRELOAD 내용은 Makefile 참고