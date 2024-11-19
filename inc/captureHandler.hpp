#ifndef JIKIMZON_CAPTUREHANDLER_H
#define JIKIMZON_CAPTUREHANDLER_H

#include <iostream>

#define WIDTH   1280
#define HEIGHT  720

class CaptureHandler {
public:
    CaptureHandler() = default;
    ~CaptureHandler() = default;

    void InitCapture(int, int, int, int);
    bool GetFrame(cv::Mat&);
    void StartCapture();

private:
    cv::VideoCapture mCap;
    
};

#endif