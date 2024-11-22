#ifndef JIKIMZON_DATA_H
#define JIKIMZON_DATA_H

#include <iostream>
#include <cstdint>
#include <opencv2/opencv.hpp>

// class frame
namespace data
{
    struct FrameHeader
    {
        uint8_t    frameId;            // 프레임 아이디 - 1001
        uint8_t    frameSize;          // 프레임 사이즈 - 2002
        char       timestamp[19];     // 타임스탬프 - "20241122_123456.789"
    };
    // sizeof(FrameHeader) = 21


    // functions
    void SerializeFrameHeader(data::FrameHeader& frameHeader, std::vector<uint8_t>& buffer);
    void DeserializeFrameHeader(std::vector<uint8_t>& buffer, data::FrameHeader& frameHeader);
}

#endif