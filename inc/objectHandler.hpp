#ifndef JIKIMZON_OBJECTHANDLER_H
#define JIKIMZON_OBJECTHANDLER_H

#include "data.hpp"

#include <opencv2/opencv.hpp>

class ObjectHandler {
public:
    ObjectHandler() = default;
    ~ObjectHandler() = default;

    void InitModel(const std::string&, const cv::Size &inputShape = {640, 640});
    std::vector<data::Detection> DetectObject(cv::Mat&, std::string&);

private:
    cv::dnn::Net mYoloNet;
    cv::Size mModelInputShape   = {640, 640};
    float mConfidenceThreshold  = 0.65;
    float mScoreThreshold       = 0.55;
    float mNMSThreshold         = 0.50;
    std::vector<std::string> mObjClasses = {
        "biodegradable", "cardboard", "glass", "metal", "paper", "plastic"
    };
};

#endif