#ifndef JIKIMZON_OBJECTHANDLER_H
#define JIKIMZON_OBJECTHANDLER_H

#include <opencv2/opencv.hpp>

namespace object
{
    struct Detection
    {
        int         classId;
        std::string className;
        float       confidence;
        cv::Rect    box;
    };
}


class ObjectHandler {
public:
    ObjectHandler() = default;
    ~ObjectHandler() = default;

    void InitModel(const std::string&, const cv::Size &inputShape = {640, 640});
    std::vector<object::Detection> DetectObject(cv::Mat&);

private:
    cv::dnn::Net mYoloNet;
    cv::Size mModelInputShape   = {640, 640};
    float mConfidenceThreshold  = 0.25;
    float mScoreThreshold       = 0.45;
    float mNMSThreshold         = 0.50;
    std::vector<std::string> mObjClasses = {
        "biodegradable", "cardboard", "glass", "metal", "paper", "plastic"
    };
};

#endif