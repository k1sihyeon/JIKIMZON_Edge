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

cv::Mat _getFrame(cv::VideoCapture* cap)
{
    cv::Mat frame;

     while (!cap->read(frame))
    {
        if (!frame.empty())
        {
            break;
        }
    }

    return frame;
}

}

// 외부 함수
extern "C" void InitCapture(void** cap, int camIdx, int width, int height, int fps)
{
    auto* capture = new cv::VideoCapture();
    _initCapture(capture, camIdx, width, height, fps);
    *cap = static_cast<void*>(capture);
}

extern "C" void GetFrame(void* cap, unsigned char** buffer, int* rows, int* cols, int* channels)
{
    auto* capture = static_cast<cv::VideoCapture*>(cap);
    cv::Mat frame = _getFrame(capture);

    *rows = frame.rows;
    *cols = frame.cols;
    *channels = frame.channels();

    size_t dataSize = frame.total() * frame.elemSize();
    *buffer = new unsigned char[dataSize];
    std::memcpy(*buffer, frame.data, dataSize);
}

extern "C" void externalFunction(int value)
{
    internalFunction(value);
}
