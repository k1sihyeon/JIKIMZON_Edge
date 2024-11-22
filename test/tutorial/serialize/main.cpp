#include <iostream>
#include <cstring>

#include "frame.hpp"

int main()
{   
    frame::Frame frame;
    std::vector<uint8_t> outBuffer;

    struct frame::Header header;
    header.frameId = 1001;
    header.bodySize = 2002;
    header.imageWidth = 1280;
    header.imageHeight = 720;
    header.imageFormat = frame::ImageFormat::RAW;
    std::string timestamp = "20241122_123456.789";
    std::memcpy(header.timestamp, timestamp.c_str(), 19);
    std::memset(header.padding1, 0, 3);
    std::memset(header.padding2, 0, 1);

    frame.SetHeader(header);

    frame.Serialize(outBuffer);

    frame::Frame outFrame;
    outFrame.Deserialize(outBuffer);
    
    frame::Header outHeader = outFrame.GetHeader();
    std::cout << "frameId: " << static_cast<int>(outHeader.frameId) << std::endl;
    std::cout << "bodySize: " << static_cast<int>(outHeader.bodySize) << std::endl;
    std::cout << "imageWidth: " << static_cast<int>(outHeader.imageWidth) << std::endl;
    std::cout << "imageHeight: " << static_cast<int>(outHeader.imageHeight) << std::endl;
    std::cout << "imageFormat: " << static_cast<int>(outHeader.imageFormat) << std::endl;
    std::cout << "timestamp: " << outHeader.timestamp << std::endl;
    
    // !!!! imageFormat에는 정적 캐스트 필수 !!!!

    return 0;
}