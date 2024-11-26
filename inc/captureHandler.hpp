#ifndef JIKIMZON_CAPTUREHANDLER_H
#define JIKIMZON_CAPTUREHANDLER_H

#include "objectHandler.hpp"

#include <opencv2/opencv.hpp>

class CaptureHandler {
public:
    CaptureHandler() = default;
    ~CaptureHandler() = default;

    void InitCapture(int, int, int, int);
    bool GetFrame(cv::Mat&);
    void ShowFrame(cv::Mat&);
    void ShowFrame(cv::Mat&, object::Detection&);

private:
    cv::VideoCapture mCap;
};

#endif