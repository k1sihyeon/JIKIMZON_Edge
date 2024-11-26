#include <iostream>

#include "tcpHandler.hpp"
#include "data.hpp"

int main()
{   
    // 예제 Frame 객체
    data::Frame frameData;
    frameData.timestamp = data::GetCurrentTime();
    frameData.encryptedFrame = new uint8_t[data::FRAME_SIZE];
    std::fill(frameData.encryptedFrame, frameData.encryptedFrame + data::FRAME_SIZE, 0);

    // buffer 선언
    std::vector<uint8_t> buffer;

    // Frame 객체 직렬화
    data::SerializeFrame(frameData, buffer);

    // 여기서, 엣지의 경우 TCP send
    // 서버의 경우 TCP recv

    // Frame 객체 역직렬화
    data::Frame deserializedFrame;
    data::DeserializeFrame(buffer, deserializedFrame);

    std::cout << "[Before serialization] timestamp: " << frameData.timestamp << std::endl;
    std::cout << "[Before serialization] frame: " << frameData.encryptedFrame[20] << std::endl;
    std::cout << "[After serialization] timestamp: " << deserializedFrame.timestamp << std::endl;
    std::cout << "[After serialization] frame: " << deserializedFrame.encryptedFrame[20] << std::endl;

    ///////////////////////////////////////////////////////////////////
    // 예제 Detection 객체
    data::Detection detection;
    detection.timeStamp = data::GetCurrentTime();
    detection.classId = 0;
    detection.className = "plastic";
    detection.confidence = 0.95;
    detection.box = cv::Rect(0, 0, 100, 100);

    // buffer 초기화
    buffer.clear();

    // Detection 객체 직렬화
    data::SerializeDetection(detection, buffer);

    // 여기서, 엣지의 경우 TCP send
    // 서버의 경우 TCP recv

    // Detection 객체 역직렬화
    data::Detection deserializedDetection;
    data::DeserializeDetection(buffer, deserializedDetection);

    std::cout << "[Before serialization] class: " << detection.className << ", confidence: " << detection.confidence << std::endl;
    std::cout << "[Before serialization] confidence: " << detection.confidence << std::endl;

    std::cout << "[After serialization] class: " << deserializedDetection.className << ", confidence: " << deserializedDetection.confidence << std::endl;
    std::cout << "[After serialization] confidence: " << deserializedDetection.confidence << std::endl; 

    return 0;
}