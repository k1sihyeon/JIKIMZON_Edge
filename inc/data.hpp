#ifndef JIKIMZON_DATA_H
#define JIKIMZON_DATA_H

#include <iostream>
#include <opencv2/opencv.hpp>

namespace data
{
    struct Frame
    {
        std::string     timestamp;          // 타임스탬프                // 19
        uint8_t         *encryptedFrame;    // 암호화된 프레임 데이터    // 2764800 = width * height * channels(3)
    };

    struct Detection
    {
        std::string timeStamp;
        int         classId;
        std::string className;
        float       confidence;
        cv::Rect    box;
    };

    // size
    const int TIMESTAMP_SIZE    = 19;
    const int IV_SIZE           = 12;
    const int FRAME_SIZE        = 2764800;
    const int CLASSNAME_SIZE    = 13;

    // functions
    std::string GetCurrentTime();
    std::string PadFixedLength(std::string& str, int length);
    
    void SerializeFrame(std::string timestamp, uint8_t *encryptedFrame, std::vector<uint8_t>& buffer);
    void SerializeFrame(data::Frame& frameData, std::vector<uint8_t>& buffer);
    void DeserializeFrame(std::vector<uint8_t>& buffer, data::Frame& frameData);

    void SerializeDetection(data::Detection& detection, std::vector<uint8_t>& buffer);
    void DeserializeDetection(std::vector<uint8_t>& buffer, data::Detection& detection);
}

#endif