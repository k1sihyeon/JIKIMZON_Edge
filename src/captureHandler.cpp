#include "captureHandler.hpp"
#include "tcpHandler.hpp"
#include "encodeHandler.hpp"
#include "objectHandler.hpp"

#include <unistd.h>
#include <limits.h>

#include <opencv2/opencv.hpp>

void CaptureHandler::InitCapture(int camIdx, int width, int height, int fps)
{
    mCap = cv::VideoCapture(camIdx, cv::CAP_V4L2);

    if (!mCap.isOpened()) {
        std::cerr << "mCap is not opened" << std::endl;
        exit(EXIT_FAILURE);
    }

    mCap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    mCap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    mCap.set(cv::CAP_PROP_FPS, fps);
}

bool CaptureHandler::GetFrame(cv::Mat& frame)
{
    if (!mCap.read(frame) || frame.empty())
    {
        return false;
    }

    return true;
}