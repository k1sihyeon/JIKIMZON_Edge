#ifndef JIKIMZON_DATA_H
#define JIKIMZON_DATA_H

#include <iostream>
#include <opencv2/opencv.hpp>

namespace data
{
    struct FrameHeader
    {
        unsigned int    frameId;            // 프레임 아이디
        size_t          frameSize;          // 프레임 사이즈
        // unsigned int    width;              // 프레임 너비
        // unsigned int    height;             // 프레임 높이
        char            timestamp[19];      // 타임스탬프                // 19
    };

    // struct Frame
    // {
    //     unsigned int    frameId;            // 프레임 아이디
    //     size_t          frameSize;          // 프레임 사이즈
    //     std::string     timestamp;          // 타임스탬프                // 19
    //     cv::Mat         frame;              // 프레임 데이터

    //     //uint8_t         *encryptedFrame;    // 암호화된 프레임 데이터
    // };

    // size
    //const int TIMESTAMP_SIZE    = sizeof(char) * 19;
    // const int FRAME_HEADER_SIZE = sizeof(unsigned int) + sizeof(size_t) + (sizeof(char) * 19);
    

    // functions
    void SerializeFrameHeader(data::FrameHeader& frameHeader, std::vector<uint8_t>& buffer);
    void DeserializeFrameHeader(std::vector<uint8_t>& buffer, data::FrameHeader& frameHeader);
}

#endif