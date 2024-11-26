#include "captureHandler.hpp"
#include "tcpHandler.hpp"
#include "encodeHandler.hpp"
#include "objectHandler.hpp"
#include "cipherHandler.hpp"
#include "frame.hpp"
#include "utils.hpp"

#include <cstdint>
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>

#define OUT

int main()
{
    const int width     = 1280;
    const int height    = 720;
    const int fps       = 15;
    const int bitrate   = 1000000;

    Utils utils;
    CaptureHandler capHandler;
    TcpHandler tcpHandler;
    ObjectHandler objHandler;
    EncodeHandler encodeHandler(width, height, bitrate, fps);
    CipherHandler cipherHandler;

    // Get current working directory
    std::string path = utils.GetWorkingDir();

    capHandler.InitCapture(0, width, height, fps);   // camIdx, width, height, fps
    tcpHandler.InitSocket();
    objHandler.InitModel(path + "/res/yolov5n-garbage.onnx");

    cv::Mat inFrame;
    uint32_t frameId = 0;
    std::string timestamp;
    std::vector<uint8_t> encodedFrame;
    std::vector<uint8_t> encryptedFrame;
    unsigned char iv[12];
    object::Detection detections;
    nlohmann::json json;

    frame::HeaderStruct headerStruct;
    frame::Header header;
    frame::Body body;
    frame::Frame frame;
    std::vector<uint8_t> buffer;

    while (true) 
    {
        if (!capHandler.GetFrame(inFrame))
        {
            continue;
        }

        // 현재 시간
        timestamp = utils.GetCurrentTime();

        // TODO: 전처리
        

        // 모델 추론
        objHandler.DetectObject(inFrame, timestamp, OUT detections);

        // 탐지 결과 JSON 전송
        objHandler.CreateJson(detections, OUT json);
        //tcpHandler.SendJson(json);
            //tcpHandler.SendJson(objHandler.CreateJson(detections));

        // h.264 압축
        encodeHandler.EncodeFrame(inFrame, OUT encodedFrame);
        
        // 암호화
        encryptedFrame.resize(encodedFrame.size());
        cipherHandler.EncryptData(timestamp, encodedFrame, encodedFrame.size(), OUT encryptedFrame);
      
        // frame header 설정
        headerStruct.frameId    = static_cast<uint32_t>(frameId);
        headerStruct.bodySize   = static_cast<uint32_t>(encryptedFrame.size());
        headerStruct.imageWidth = static_cast<uint16_t>(width);
        headerStruct.imageHeight = static_cast<uint16_t>(height);
        headerStruct.imageFormat = frame::ImageFormat::H264;
        std::strcpy(headerStruct.timestamp, timestamp.c_str());

        header.SetHeader(headerStruct);
        
        // frame body 설정
        body.SetImage(encryptedFrame);

        // Frame 객체 설정
        frame.SetFrame(header, body);

        // Frame 객체 Serialize 후 전송
        frame.Serialize(OUT buffer);
        tcpHandler.SendData(buffer);

        // 디버깅용 화면 출력
        // capHandler.ShowFrame(inFrame, detections); // capHandler.ShowFrame(inFrame);

        // frame ID 증가
        frameId += 1;
    }
}