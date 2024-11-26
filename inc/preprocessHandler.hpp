#ifndef JIKIMZON_PREPROCESSHANDLER_H
#define JIKIMZON_PREPROCESSHANDLER_H

#include <iostream>
#include <opencv2/opencv.hpp>

class PreprocessHandler {
public:
    PreprocessHandler(cv::Mat& src, cv::Mat& dest);
    ~PreprocessHandler() = default;
    
    void Threading();

private:
    cv::Mat& mSrc;
    cv::Mat& mDest;
    cv::Mat mCumulativeHist;

    void exposure(const int start, const int end);
};

#endif