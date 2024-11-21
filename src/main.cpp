#include "captureHandler.hpp"
#include "tcpHandler.hpp"
#include "encodeHandler.hpp"
#include "objectHandler.hpp"

#include "data.hpp"
#include "cipherHandler.hpp"

#include <unistd.h>
#include <limits.h>
#include <nlohmann/json.hpp>

#include <opencv2/opencv.hpp>

int main()
{
    const int width     = 1280;
    const int height    = 720;
    const int fps       = 15;
    const int bitrate   = 1000000;

    // Get current working directory
    char buf[PATH_MAX];
    getcwd(buf, PATH_MAX);
    std::string path(buf);

    Utils utils;
    CaptureHandler capHandler;
    TcpHandler tcpHandler;
    ObjectHandler objHandler;
    EncodeHandler encodeHandler(width, height, bitrate, fps);
    CipherHandler cipherHandler;

    capHandler.InitCapture(0, width, height, fps);   // camIdx, width, height, fps
    tcpHandler.InitSocket();
    objHandler.InitModel(path + "/res/yolov5n-garbage.onnx");

    std::vector<uint8_t> encodedFrame;
    std::vector<uint8_t> encryptedFrame;
    std::vector<uint8_t> decryptedFrame;

    while (true) 
    {
        cv::Mat inFrame;
        if (!capHandler.GetFrame(inFrame))
        {
            continue;
        }

        std::string timestamp = data::GetCurrentTime();
        std::cout << "===== timestamp: " << timestamp << " =====" << std::endl;

        // TODO: 전처리
        
        // 모델 추론
        object::Detection detections;
        detections = objHandler.DetectObject(inFrame);


        // 디버깅용 화면 출력
        capHandler.ShowFrame(inFrame, detections);


        // TODO: 결과 파싱, json화, 전송
        nlohmann::json json = objHandler.CreateJson(detections);

        // h.264 압축
        encodeHandler.EncodeFrame(inFrame, encodedFrame);
        
        // 암호화 && tcp 전송
        auto key = cipherHandler.Init();

        // tcpHandler.SendData(key, (size_t)32UL); 

        // cipherHandler.EncryptData(encodedFrame, sizeof(encodedFrame), encryptedFrame, decryptedFrame);
        auto iv = cipherHandler.EncryptData(encodedFrame, sizeof(encodedFrame), encryptedFrame);
        tcpHandler.SendData(iv, (size_t)12UL); 

        // if (cipherHandler.isEqual(encodedFrame, decryptedFrame, sizeof(encodedFrame)))
        // {
        //     std::cout<<"true";
        // }

        // tcp 전송
        tcpHandler.SendData(encryptedFrame); 
    }
}