#include "v4l2Handler.hpp"
#include "tcpHandler.hpp"

#include <opencv2/opencv.hpp>


V4L2Handler* V4L2Handler::instance = nullptr;

V4L2Handler::V4L2Handler() {}
V4L2Handler::~V4L2Handler() {}

void V4L2Handler::StartCapture()
{
    cv::VideoCapture cap(0, cv::CAP_V4L2);
    
    if (!cap.isOpened()) {
        std::cerr << "cap is not opened" << std::endl;
        exit(EXIT_FAILURE);
    }
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
    cap.set(cv::CAP_PROP_FPS, 30);

    cv::Mat frame;
    // cap >> frame;
    // std::cout << frame.type() << std::endl;

    while (true) {
        if (!cap.read(frame) || frame.empty()) {
            continue;
        }
        // 전처리
        
        // h.264 압축

        // tcp 전송
        TcpHandler::GetInstance()->SendFrame(frame);
    }

}