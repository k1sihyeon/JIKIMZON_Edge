#ifndef JIKIMZON_PREPROCESSHANDLER_H
#define JIKIMZON_PREPROCESSHANDLER_H

#include <iostream>
#include <opencv2/opencv.hpp>

class PreprocessHandler {
public:
    PreprocessHandler(cv::Mat& src, cv::Mat& dest);
    ~PreprocessHandler();
    
    void Threading();
    void Exposure(const int start, const int end);

private:
    cv::Mat& src;
    cv::Mat& dest;

    cv::Mat cumulativeHist;
};

#endif