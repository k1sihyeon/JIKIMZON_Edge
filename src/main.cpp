#include "captureHandler.hpp"
#include "tcpHandler.hpp"
#include "encodeHandler.hpp"
#include "objectHandler.hpp"
#include "cipherHandler.hpp"
#include "frame.hpp"
#include "utils.hpp"

#include <unistd.h>
#include <limits.h>
#include <cstdint>
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

    std::vector<uint8_t> buffer;
    uint32_t frameId = 0;

    while (true) 
    {
        cv::Mat inFrame;
        if (!capHandler.GetFrame(inFrame))
        {
            continue;
        }

        // 현재 시간
        std::string timestamp = utils.GetCurrentTime();

        // TODO: 전처리
        

        // 모델 추론
        object::Detection detections;
        detections = objHandler.DetectObject(inFrame, timestamp);

        // 탐지 결과 JSON 전송
        nlohmann::json json = objHandler.CreateJson(detections);
        tcpHandler.SendJson(json);
            //tcpHandler.SendJson(objHandler.CreateJson(detections));

        // h.264 압축
        encodeHandler.EncodeFrame(inFrame, encodedFrame);
        
        // 암호화
        encryptedFrame.resize(encodedFrame.size());
        cipherHandler.EncryptData(timestamp, encodedFrame, encodedFrame.size(), encryptedFrame);
      
        // frame header 설정
        frame::HeaderStruct headerStruct {
            .frameId    = static_cast<uint32_t>(frameId),
            .bodySize   = static_cast<uint32_t>(encryptedFrame.size()),
            .imageWidth = static_cast<uint16_t>(width),
            .imageHeight = static_cast<uint16_t>(height),
            .imageFormat = frame::ImageFormat::H264,
        };
        std::strcpy(headerStruct.timestamp, timestamp.c_str());

        frame::Header header(headerStruct);
        
        // frame body 설정
        frame::Body body(encryptedFrame);

        // Frame 객체 생성
        frame::Frame frame(header, body);

        // Frame 객체 Serialize 후 전송
        frame.Serialize(buffer);
        tcpHandler.SendData(buffer);

        // 디버깅용 화면 출력
        // capHandler.ShowFrame(inFrame, detections); // capHandler.ShowFrame(inFrame);

        // frame ID 증가
        frameId += 1;
    }
}