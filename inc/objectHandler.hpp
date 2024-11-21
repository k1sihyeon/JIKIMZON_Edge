#ifndef JIKIMZON_OBJECTHANDLER_H
#define JIKIMZON_OBJECTHANDLER_H

#include <opencv2/opencv.hpp>

namespace object
{
    struct Obj
    {
        std::string className;
        cv::Rect    box;
    };

    struct Detection
    {
        // std::string timeStamp;
        std::vector<Obj> vObj;
    };
}

class ObjectHandler {
public:
    ObjectHandler() = default;
    ~ObjectHandler() = default;

    void InitModel(const std::string&, const cv::Size &inputShape = {640, 640});
    object::Detection DetectObject(cv::Mat&);

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