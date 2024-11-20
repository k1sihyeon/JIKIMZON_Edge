#ifndef JIKIMZON_DATA_H
#define JIKIMZON_DATA_H

#include <iostream>
#include <opencv2/opencv.hpp>

namespace data
{
    struct Detection
    {
        int         classId;
        std::string className;
        float       confidence;
        cv::Rect    box;
    };

    struct FrameData
    {
        std::string timestamp;          // 타임스탬프
        std::vector<uint8_t> frame;     // 프레임 데이터
    };
	
    struct DetectData
    {
		std::string timestamp;          // 타임스탬프
		data::Detection detection;      // 객체 탐지 결과
    };

    std::string GetCurrentTime();
    
}

#endif