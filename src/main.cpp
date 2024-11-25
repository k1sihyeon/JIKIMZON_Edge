#include "captureHandler.hpp"
#include "tcpHandler.hpp"
#include "encodeHandler.hpp"
#include "objectHandler.hpp"
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

    while (true) 
    {
        cv::Mat inFrame;
        if (!capHandler.GetFrame(inFrame))
        {
            continue;
        }

        std::string timestamp = utils.GetCurrentTime();

        // TODO: 전처리

        // 모델 추론
        object::Detection detections = objHandler.DetectObject(inFrame);

        // TODO: 결과 파싱, json화, 전송
        nlohmann::json json = objHandler.CreateJson(detections);
        
        // h.264 압축
        encodeHandler.EncodeFrame(inFrame, encodedFrame);
        
        // 암호화 && tcp 전송
        encryptedFrame.resize(encodedFrame.size());
        cipherHandler.EncryptData(timestamp, encodedFrame, encodedFrame.size(), encryptedFrame);

        // tcp 전송
        // tcpHandler.SendData(encryptedFrame); 
    }
}