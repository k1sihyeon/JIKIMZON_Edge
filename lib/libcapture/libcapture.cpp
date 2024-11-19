#include "libcapture.h"
#include <opencv2/opencv.hpp>
#include <iostream>

namespace
{

void internalFunction(int value)
{
    std::cout << "Internal function called with value: " << value << std::endl;
}

void _initCapture(cv::VideoCapture* cap, int camIdx, int width, int height, int fps)
{
    cap->open(camIdx, cv::CAP_V4L2);

    if (!cap->isOpened())
    {
        std::cerr << "Capture device not opened!" << std::endl;
        exit(EXIT_FAILURE);
    }

    cap->set(cv::CAP_PROP_FRAME_WIDTH, width);
    cap->set(cv::CAP_PROP_FRAME_HEIGHT, height);
    cap->set(cv::CAP_PROP_FPS, fps);
}

bool _getFrame(cv::VideoCapture* cap, cv::Mat& frame)
{
    if (!cap->read(frame) || frame.empty())
    {
        return false;
    }

    return true;
}

}

// 외부 함수
extern "C" void InitCapture(void** cap, int camIdx, int width, int height, int fps)
{
    auto* capture = new cv::VideoCapture();
    _initCapture(capture, camIdx, width, height, fps);
    *cap = static_cast<void*>(capture);
}

extern "C" bool GetFrame(void* cap, uint8_t** buffer)
{
    auto* capture = static_cast<cv::VideoCapture*>(cap);
    cv::Mat frame;

    if (_getFrame(capture, frame))
    {
        int dataSize = frame.cols * frame.rows * frame.elemSize();
        *buffer = new uint8_t[dataSize];
        std::memcpy(*buffer, frame.data, dataSize);
        
        return true;
    }
    else
    {
        return false;
    }
}

extern "C" void externalFunction(int value)
{
    internalFunction(value);
}
