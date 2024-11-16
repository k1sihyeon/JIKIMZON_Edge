#include "objectHandler.hpp"

#include <opencv2/opencv.hpp>

ObjectHandler* ObjectHandler::sInstance = nullptr;

ObjectHandler& ObjectHandler::GetInstance()
{
    if (sInstance == nullptr)
    {
        sInstance = new ObjectHandler;
    }
    return *sInstance;
}

void ObjectHandler::InitModel(std::string path)
{
    try
    {
        //mMobileNet = cv::dnn::readNetFromTensorflow(path);
        mMobileNet = cv::dnn::readNetFromONNX(path);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Initmodel: \n" << e.what() << '\n';
    }
    
    
}

cv::Mat ObjectHandler::DetectObject(cv::Mat& frame)
{
    // 이미지를 모델 입력 크기에 맞추기
    cv::Mat blob = cv::dnn::blobFromImage(frame, 1 / 255.0, cv::Size(640, 640), cv::Scalar(0, 0, 0), true, false);
    
    try
    {
        // 모델에 이미지 입력
        mMobileNet.setInput(blob);
    }
    catch(const std::exception& e)
    {
        std::cerr << "setInput: \n" << e.what() << '\n';
    }
    
    cv::Mat output;

    std::vector<cv::Mat> outputs;


    try
    {
        // 모델 추론
        ///*cv::Mat*/ output = mMobileNet.forward();
        mMobileNet.forward(outputs, mMobileNet.getUnconnectedOutLayersNames());

        if (outputs.empty())
        {
            std::cerr << "output is empty" << std::endl;
            return output;
        }

        float* data = (float*)outputs[0].data;
        const int dimensions = 85;            // YOLOv5의 출력 차원
        const int rows = outputs[0].size[1];  // 탐지된 객체 수

        for (int i = 0; i < rows; ++i)
        {
            float confidence = data[i * dimensions + 4];
            if (confidence > 500)
            {
                // 바운딩 박스 좌표 변환 (x_center, y_center, width, height -> x1, y1, x2, y2)
                float x_center = data[i * dimensions + 0] * frame.cols;
                float y_center = data[i * dimensions + 1] * frame.rows;
                float width = data[i * dimensions + 2] * frame.cols;
                float height = data[i * dimensions + 3] * frame.rows;

                float x1 = x_center - width / 2;
                float y1 = y_center - height / 2;
                float x2 = x_center + width / 2;
                float y2 = y_center + height / 2;

                // 클래스 점수 계산
                cv::Mat scores(1, dimensions - 5, CV_32FC1, data + i * dimensions + 5);
                cv::Point classIdPoint;
                double maxClassScore;
                minMaxLoc(scores, 0, &maxClassScore, 0, &classIdPoint);
                int classId = classIdPoint.x;

                // 필터링
                if (maxClassScore > 0.3 /*SCORE_THRESHOLD*/) {
                    // 결과 저장
                    //detections.push_back({x1, y1, x2, y2, confidence, classId});

                    std::cout << "classId: " << classId << std::endl;
                    std::cout << "confidence: " << confidence << std::endl;
                    std::cout << "maxClassScore: " << maxClassScore << std::endl;
                    std::cout << "x1: " << x1 << std::endl;
                    std::cout << "y1: " << y1 << std::endl;
                    std::cout << "x2: " << x2 << std::endl;
                    std::cout << "y2: " << y2 << std::endl;
                }
            }
        }

    }
    catch(const std::exception& e)
    {
        std::cerr << "forward: \n" << e.what() << '\n';
    }
    

    

    return output;
}