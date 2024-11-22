#include "captureHandler.hpp"
#include "objectHandler.hpp"

#include <unistd.h>
#include <limits.h>

#include <opencv2/opencv.hpp>

void CaptureHandler::InitCapture(int camIdx, int width, int height, int fps)
{
    mCap = cv::VideoCapture(camIdx, cv::CAP_V4L2);

    if (!mCap.isOpened())
    {
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

void CaptureHandler::ShowFrame(cv::Mat& inFrame)
{
    cv::imshow("raw frame", inFrame);
    cv::waitKey(10);
}

void CaptureHandler::ShowFrame(cv::Mat& inFrame, std::vector<data::Detection>& detections)
{
    for (const auto& detection : detections)
    {
        cv::Rect box = detection.box;
        cv::Scalar color = cv::Scalar(125, 125, 0);

        // Detection box
        cv::rectangle(inFrame, box, color, 2);

        // Detection box text
        std::string classString = detection.className + ' ' + std::to_string(detection.confidence).substr(0, 4);
        cv::Size textSize = cv::getTextSize(classString, cv::FONT_HERSHEY_DUPLEX, 1, 2, 0);
        cv::Rect textBox(box.x, box.y - 40, textSize.width + 10, textSize.height + 20);

        cv::rectangle(inFrame, textBox, color, cv::FILLED);
        cv::putText(inFrame, classString, cv::Point(box.x + 5, box.y - 10), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 0), 2, 0);
    }

    // // This is only for preview purposes
    // float scale = 0.8;
    // cv::resize(inFrame, inFrame, cv::Size(inFrame.cols*scale, inFrame.rows*scale));
    cv::imshow("Detection", inFrame);
    cv::waitKey(10);
}
