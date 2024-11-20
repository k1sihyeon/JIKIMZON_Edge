#include "captureHandler.hpp"
#include "tcpHandler.hpp"
#include "encodeHandler.hpp"
#include "objectHandler.hpp"
#include "data.hpp"

#include <unistd.h>
#include <limits.h>

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

    capHandler.InitCapture(0, width, height, fps);   // camIdx, width, height, fps
    tcpHandler.InitSocket();
    objHandler.InitModel(path + "/res/yolov5n-garbage.onnx");

    std::vector<uint8_t> encodedFrame;

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
        std::vector<data::Detection> detections;
        detections = objHandler.DetectObject(inFrame, timestamp);
        
        for (auto& detection : detections)
        {
            std::vector<uint8_t> buffer;
            data::SerializeDetection(detection, buffer);
            
            data::Detection deserializedDetection;
            data::DeserializeDetection(buffer, deserializedDetection);

            std::cout << "[before serialization] class: " << detection.className << ", confidence: " << detection.confidence << std::endl;
            std::cout << "[after serialization] class: " << deserializedDetection.className << ", confidence: " << deserializedDetection.confidence << std::endl;    
        }

        // 디버깅용 화면 출력
        capHandler.ShowFrame(inFrame, detections);

        // TODO: 결과 파싱, json화, 전송
        
        // h.264 압축
        encodeHandler.EncodeFrame(inFrame, encodedFrame);

        // TODO: 암호화
        
        // tcp 전송
        tcpHandler.SendFrame(encodedFrame); 
        // 직렬화 후 전송
        //uint8_t encryptedFrame[width * height * 3];     // 2764800
    }
}