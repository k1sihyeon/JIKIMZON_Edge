#include "captureHandler.hpp"
#include "tcpHandler.hpp"
#include "encodeHandler.hpp"
#include "objectHandler.hpp"

#include <unistd.h>
#include <limits.h>

#include <opencv2/opencv.hpp>

CaptureHandler* CaptureHandler::sInstance = nullptr;

CaptureHandler& CaptureHandler::GetInstance()
{
    if (sInstance == nullptr)
    {
        sInstance = new CaptureHandler;
    }
    return *sInstance;
}

void CaptureHandler::StartCapture()
{
    cv::VideoCapture cap(0, cv::CAP_V4L2);
    
    if (!cap.isOpened()) {
        std::cerr << "cap is not opened" << std::endl;
        exit(EXIT_FAILURE);
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, WIDTH);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, HEIGHT);
    cap.set(cv::CAP_PROP_FPS, 30);

    TcpHandler::GetInstance().InitSocket();

    char buf[PATH_MAX];
    getcwd(buf, PATH_MAX);
    std::string path(buf);
    ObjectHandler::GetInstance().InitModel(path + "/res/best.onnx");

    int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));

    std::vector<uint8_t> encodedFrame;

    cv::Mat inFrame;

    while (true) {
        if (!cap.read(inFrame))
        {
            //std::cerr << "Failed to capture frame" << std::endl;
            continue;
        }

        if (inFrame.empty())
        {
            //std::cerr << "Empty frame" << std::endl;
            continue;
        }

        // TODO: 전처리

        // 모델 추론
        std::vector<object::Detection> detections;
        detections = ObjectHandler::GetInstance().DetectObject(inFrame);
        
        for (const auto& detection : detections)
        {
            std::cout << "class: " << detection.className << ", confidence: " << detection.confidence << std::endl;
        }

        // TODO: 결과 파싱, json화, 전송
        
        // h.264 압축
        EncodeHandler::GetInstance(width, height, 1000000, 30)->encodeFrame(inFrame, encodedFrame);

        // TODO: 암호화
        
        // tcp 전송
        TcpHandler::GetInstance().SendFrame(encodedFrame);
    }
}