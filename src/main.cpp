#include "captureHandler.hpp"
#include "tcpHandler.hpp"
#include "encodeHandler.hpp"
#include "objectHandler.hpp"

#include "frame.hpp"
#include "utils.hpp"
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
    frame::Frame frame;
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

    std::vector<uint8_t> buffer;
    uint32_t frameId = 0;
    //frame::Frame frame;

    while (true) 
    {
        cv::Mat inFrame;
        if (!capHandler.GetFrame(inFrame))
        {
            continue;
        }

        // //std::string timestamp = utils.GetCurrentTime();
        // std::string timestamp = "20241122_123456.789";
        // struct frame::Header header;
        // header.frameId = static_cast<uint32_t>(frameId);
        // header.bodySize = static_cast<uint32_t>(encryptedFrame.size());
        // header.imageWidth = static_cast<uint16_t>(width);
        // header.imageHeight = static_cast<uint16_t>(height);
        // header.imageFormat = frame::ImageFormat::H264;
        // std::strcpy(header.timestamp, timestamp.c_str());
        // tcpHandler.SendData(buffer);

        //frame.DeserializeFrameHeader

        // TODO: 전처리
        
        // 모델 추론
        // object::Detection detections;
        // detections = objHandler.DetectObject(inFrame);


        // 디버깅용 화면 출력
        // capHandler.ShowFrame(inFrame, detections);


        // TODO: 결과 파싱, json화, 전송
        // nlohmann::json json = objHandler.CreateJson(detections);

        // h.264 압축
        // encodeHandler.EncodeFrame(inFrame, encodedFrame);
        
        // 암호화 && tcp 전송
        // auto key = cipherHandler.Init();

        // tcpHandler.SendData(key, (size_t)32UL); 

        // cipherHandler.EncryptData(encodedFrame, sizeof(encodedFrame), encryptedFrame, decryptedFrame);
        // auto iv = cipherHandler.EncryptData(encodedFrame, sizeof(encodedFrame), encryptedFrame);
        // tcpHandler.SendData(iv, (size_t)12UL); 

        // if (cipherHandler.isEqual(encodedFrame, decryptedFrame, sizeof(encodedFrame)))
        // {
        //     std::cout<<"true";
        // }

        //std::string timestamp = utils.GetCurrentTime();
        std::string timestamp = "20241122_123456.789";
        struct frame::Header header;
        header.frameId = static_cast<uint32_t>(1001);
        header.bodySize = static_cast<uint32_t>(100);
        header.imageWidth = static_cast<uint16_t>(width);
        header.imageHeight = static_cast<uint16_t>(height);
        header.imageFormat = frame::ImageFormat::H264;
        std::strcpy(header.timestamp, timestamp.c_str());
        
        frame.SetHeader(header);
        frame.Serialize(buffer);

        tcpHandler.SendData(buffer);

        // tcp 전송
        std::vector<uint8_t> frameBuffer(header.bodySize, 2);
        tcpHandler.SendData(frameBuffer); 

        // frameId += 1;
    }
}