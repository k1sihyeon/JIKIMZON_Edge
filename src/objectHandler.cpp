#include "objectHandler.hpp"

#include <opencv2/opencv.hpp>

void ObjectHandler::InitModel(const std::string& path, const cv::Size &inputShape)
{
    mYoloNet = cv::dnn::readNetFromONNX(path);
    mModelInputShape = inputShape;
}

object::Detection ObjectHandler::DetectObject(cv::Mat& frame)
{
    // 이미지를 모델 입력 크기에 맞추기
    cv::Mat blob = cv::dnn::blobFromImage(frame, 1 / 255.0, mModelInputShape, cv::Scalar(0, 0, 0), true, false);
    
    mYoloNet.setInput(blob);
    
    std::vector<cv::Mat> outputs;
    std::vector<int> class_ids;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;

    // 모델 추론
    mYoloNet.forward(outputs, mYoloNet.getUnconnectedOutLayersNames());

    if (outputs.empty())
    {
        std::cerr << "output is empty" << std::endl;
        // exit(EXIT_FAILURE);
    }

    int rows = outputs[0].size[1];          // 탐지된 객체 수
    int dimensions = outputs[0].size[2];    // 탐지된 객체 정보 수
    float* data = (float*)outputs[0].data;  // 탐지된 객체 정보

    float x_factor = frame.cols / mModelInputShape.width;   // x 축 비율
    float y_factor = frame.rows / mModelInputShape.height;  // y 축 비율

    for (int i = 0; i < rows; i++)
    {
        float confidence = data[4];

        if (confidence > mConfidenceThreshold)
        {
            float* classes_scores = data + 5;

            cv::Mat scores(1, mObjClasses.size(), CV_32FC1, classes_scores);
            cv::Point class_id;
            double max_class_score;

            cv::minMaxLoc(scores, 0, &max_class_score, 0, &class_id);

            if (max_class_score > mScoreThreshold)
            {
                float x = data[0];
                float y = data[1];
                float w = data[2];
                float h = data[3];

                int left = int((x - 0.5 * w) * x_factor);
                int top = int((y - 0.5 * h) * y_factor);
                int width = int(w * x_factor);
                int height = int(h * y_factor);

                boxes.push_back(cv::Rect(left, top, width, height));
                confidences.push_back(max_class_score);
                class_ids.push_back(class_id.x);
            }
        }
        data += dimensions;
    }

    // NMS: Non-Maximum Suppression - 높은 신뢰도의 객체만 남기고 중복된 객체 제거
    std::vector<int> nms_result;
    cv::dnn::NMSBoxes(boxes, confidences, mScoreThreshold, mNMSThreshold, nms_result);

    std::vector<object::Obj> vObj;
    for (unsigned long i = 0; i < nms_result.size(); ++i)
    {
        int idx = nms_result[i];

        object::Obj obj;
        obj.className = mObjClasses[class_ids[idx]];
        obj.box = boxes[idx];

        vObj.push_back(obj);
    }

    object::Detection detection;
    // detection.timeStamp
    detection.vObj = vObj;

    return detection;
}

nlohmann::json ObjectHandler::CreateJson(object::Detection detection)
{
    nlohmann::json json;


    json["timestamp"].push_back(detection.timeStamp);

    for (auto i: detection.vObj)
    {
        nlohmann::json obj;
        obj["className"].push_back(i.className);
        obj["x"].push_back(i.box.x);
        obj["y"].push_back(i.box.y);
        obj["width"].push_back(i.box.width);
        obj["height"].push_back(i.box.height);

        json["object"].push_back(obj);
    }
    
    return json;
}