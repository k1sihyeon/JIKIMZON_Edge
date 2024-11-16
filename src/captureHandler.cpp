#include "captureHandler.hpp"
#include "tcpHandler.hpp"
#include "encodeHandler.hpp"
#include "objectHandler.hpp"

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

    //TcpHandler::GetInstance().InitSocket();
    ObjectHandler::GetInstance().InitModel("/home/sihyeon/workspace/JIKIMZON_Edge/res/best.onnx");

    int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));

    std::vector<uint8_t> encodedFrame;

    cv::Mat inFrame;
    std::vector<cv::Mat> outData;

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
        outData = ObjectHandler::GetInstance().DetectObject(inFrame);
        
        // TODO: 결과 파싱, json화, 전송
        
        // h.264 압축
        EncodeHandler::GetInstance(width, height, 1000000, 30)->encodeFrame(inFrame, encodedFrame);

        // TODO: 암호화
        
        // tcp 전송
        //TcpHandler::GetInstance().SendFrame(encodedFrame);
    }
}