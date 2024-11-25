#ifndef JIKIMZON_PREPROCESSHANDLER_H
#define JIKIMZON_PREPROCESSHANDLER_H

#include <opencv2/opencv.hpp>

class PreprocessHandler {
public:
    PreprocessHandler() = default;
    ~PreprocessHandler() = default;
    
    void adaptiveExposureControl(cv::Mat& input, cv::Mat& output);
};

#endif