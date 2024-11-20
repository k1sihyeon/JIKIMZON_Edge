#include "captureHandler.hpp"
#include "tcpHandler.hpp"
#include "encodeHandler.hpp"
#include "objectHandler.hpp"
#include "cipherHandler.hpp"

#include <unistd.h>
#include <limits.h>

#include <opencv2/opencv.hpp>

int main()
{
    const int width = 1280;
    const int height = 720;
    const int fps = 30;
    const int bitrate = 1000000;

    // Get current working directory
    char buf[PATH_MAX];
    getcwd(buf, PATH_MAX);
    std::string path(buf);

    CaptureHandler capHandler;
    TcpHandler tcpHandler;
    ObjectHandler objHandler;
    EncodeHandler encodeHandler(width, height, bitrate, fps);
    CipherHandler cipherHandler(tcpHandler.GetSock());

    capHandler.InitCapture(0, width, height, fps);   // camIdx, width, height, fps
    //tcpHandler.InitSocket();
    //objHandler.InitModel(path + "/res/yolov5n-garbage.onnx");

    std::vector<uint8_t> encodedFrame;
    // uint8_t encodedFrame[PATH_MAX];
    uint8_t encryptedFrame[width * height * 3];     // 2764800
    uint8_t decryptedFrame[width * height * 3];

    while (true) 
    {
        cv::Mat inFrame;
        if (!capHandler.GetFrame(inFrame))
        {
            continue;
        }

        // TODO: 전처리

        // 모델 추론
        // std::vector<object::Detection> detections;
        // detections = objHandler.DetectObject(inFrame);
        
        // for (const auto& detection : detections)
        // {
        //     std::cout << "class: " << detection.className << ", confidence: " << detection.confidence << std::endl;
        // }

        // TODO: 결과 파싱, json화, 전송
        
        // h.264 압축
        encodeHandler.EncodeFrame(inFrame, encodedFrame);
        std::cout << "Encoded: ";
        for (int i = 0; i < 10; i++)
        {
            std::cout << encodedFrame[i] << " ";
        }
        std:: cout << std::endl;
        
        // 암호화 전송
        cipherHandler.EncryptData(encodedFrame, sizeof(encodedFrame), encryptedFrame);
        cipherHandler.decryptData(encryptedFrame, sizeof(encryptedFrame), decryptedFrame);
        std::cout << "decrypted: ";
        for (int i = 0; i < 10; i++)
        {
            std::cout << decryptedFrame[i] << " ";
        }
        std:: cout << std::endl;
        // cipherHandler.SendEncryptedData(sizeof(encodedFrame), encryptedFrame);
        // cipherHandler.decryptData(encryptedFrame, sizeof(encryptedFrame));

        // tcp 전송
        // tcpHandler.SendFrame(encodedFrame); 
    }
}