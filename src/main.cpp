#include "captureHandler.hpp"
#include "tcpHandler.hpp"
#include "encodeHandler.hpp"
#include "objectHandler.hpp"
//#include "cipherHandler.hpp"
#include "preprocessHandler.hpp"
#include "frame.hpp"
#include "utils.hpp"
#include "tlsHandler.hpp"

#include <cstdint>
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

#define OUT

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <framePort> <jsonPort>" << std::endl;
        return EXIT_FAILURE;
    }
    
    std::string port1 = argv[1] ? argv[1] : "12345";
    std::string port2 = argv[2] ? argv[2] : "54321";

    int framePort = std::stoi(port1);
    int jsonPort = std::stoi(port2);

    const int width     = 1280;
    const int height    = 720;
    const int fps       = 15;
    const int bitrate   = 1000000;
    const int gopSize   = 10;

    Utils utils;
    CaptureHandler capHandler;
    TcpHandler frameTcpHandler;
    TcpHandler jsonTcpHandler;
    ObjectHandler objHandler;
    EncodeHandler encodeHandler(width, height, bitrate, fps, gopSize);
    //  CipherHandler cipherHandler;

    // Get current working directory
    std::string path = utils.GetWorkingDir();

    capHandler.InitCapture(0, width, height, fps);   // camIdx, width, height, fps
    int frameFd = frameTcpHandler.InitSocket(framePort);
    int jsonFd = jsonTcpHandler.InitSocket(jsonPort);
    TlsHandler frameTLS(frameFd);
    TlsHandler jsonTLS(jsonFd);
    objHandler.InitModel(path + "/res/yolov5n-garbage.onnx");

    cv::Mat inFrame;
    cv::Mat pFrame;
    uint32_t frameId = 0;
    std::string timestamp;
    std::vector<uint8_t> encodedFrame;
    //std::vector<uint8_t> encryptedFrame;
    object::Detection detections;
    nlohmann::json json;

    frame::HeaderStruct headerStruct;
    frame::Header header;
    frame::Body body;
    frame::Frame frame;
    std::vector<uint8_t> buffer;
    std::vector<uint8_t> jsonBuffer;

    while (true) 
    {
        if (!capHandler.GetFrame(inFrame))
        {
            continue;
        }

        // 현재 시간
        timestamp = utils.GetCurrentTime();

        // 전처리
        // segmentation fault
        // PreprocessHandler preproHandler(inFrame, OUT pFrame);
        // preproHandler.Threading();

        // 모델 추론
        objHandler.DetectObject(inFrame, timestamp, OUT detections);
        
        // 탐지 결과 JSON 전송
        if (!detections.vObj.empty())
        {
            objHandler.CreateJson(frameId, detections, OUT json);
            
            std::string jsonStr = json.dump() + "|";
            jsonBuffer.resize(jsonStr.size());
            std::copy(jsonStr.begin(), jsonStr.end(), jsonBuffer.begin());

            jsonTLS.SendData(jsonBuffer);
        }
        
        // h.264 압축
        encodeHandler.EncodeFrame(inFrame, OUT encodedFrame);

        // 암호화
        //encryptedFrame.resize(encodedFrame.size());
        //cipherHandler.EncryptData(timestamp, encodedFrame, encodedFrame.size(), OUT encryptedFrame);
    
        // frame header 설정
        headerStruct.frameId    = static_cast<uint32_t>(frameId);
        headerStruct.bodySize   = static_cast<uint32_t>(encodedFrame.size());
        headerStruct.imageWidth = static_cast<uint16_t>(width);
        headerStruct.imageHeight = static_cast<uint16_t>(height);
        headerStruct.imageFormat = frame::ImageFormat::H264;
        headerStruct.gopSize = static_cast<uint8_t>(gopSize);
        std::strcpy(headerStruct.timestamp, timestamp.c_str());
        if (frameId % gopSize == 0)
        {
            headerStruct.gopStartFlag = frame::GopStartFlag::START;
        }
        else
        {
            headerStruct.gopStartFlag = frame::GopStartFlag::END;
        }
        header.SetHeader(headerStruct);
        
        // frame body 설정
        frame::Body body(encodedFrame);

        // Frame 객체 설정
        frame.SetFrame(header, body);

        // Frame 객체 Serialize 후 전송
        frame.Serialize(OUT buffer);
        frameTLS.SendData(buffer);
        
        std::cout << "Frame ID: " << frameId << std::endl;
        std::cout << "Buffer Size: " << buffer.size() << std::endl;
        std::cout << "Encoded Frame body Size: " << encodedFrame.size() << std::endl;
        // std::cout << "Deserialized Frame Size: " << deserializedFrame.GetBody().GetImage().size() << std::endl;
        std::cout << "=====================================" << std::endl;
        // 디버깅용 화면 출력
        // capHandler.ShowFrame(inFrame, detections); // capHandler.ShowFrame(inFrame);

        // frame ID 증가
        frameId += 1;
    }

    return 0;
}
