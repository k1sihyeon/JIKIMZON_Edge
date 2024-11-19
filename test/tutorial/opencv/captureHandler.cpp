#include "captureHandler.hpp"

#include <opencv2/opencv.hpp>

CaptureHandler* CaptureHandler::sInstance = nullptr;

CaptureHandler& CaptureHandler::GetInstance()
{
    if (sInstance == nullptr)
    {
        sInstance = new CaptureHandler();
    }
    return *sInstance;
}

void CaptureHandler::StartCapture()
{
    // 영상 캡쳐 시작 | 0: 카메라 index | cv::CAP_V4L2: V4L2를 백엔드로 하여 영상을 가져옴
    cv::VideoCapture cap(0, cv::CAP_V4L2);
    
    if (!cap.isOpened())
    {
        std::cerr << "cap is not opened" << std::endl;
        exit(EXIT_FAILURE);
    }

    // 영상 캡쳐 Property 설정 | width, height, fps 설정
    cap.set(cv::CAP_PROP_FRAME_WIDTH, size::WIDTH);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, size::HEIGHT);
    cap.set(cv::CAP_PROP_FPS, 30);

    cv::Mat frame;

    while (true)
    {
        if (!cap.read(frame))   // 영상으로 부터 프레임 읽기
        {
            // std::cerr << "Failed to capture frame" << std::endl;
            continue;
        }

        // 무한 loop에서 연속적으로 읽고 있으므로 읽지 못하거나 빈 프레임을 읽었을 때의 처리

        if (frame.empty())
        {
            //std::cerr << "Empty frame" << std::endl;
            continue;
        }

        // GUI로 frame을 연속적으로 출력
        cv::imshow("example", frame);
        cv::waitKey(10);
    }
}