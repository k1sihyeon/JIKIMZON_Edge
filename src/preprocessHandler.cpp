#include "preprocessHandler.hpp"

void PreprocessHandler::Exposure(cv::Mat& input, cv::Mat& output)
{
    cv::Mat gray;
    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);

    int histSize = 256;
    float range[] = {0, 256};
    const float* histRange = {range};
    cv::Mat hist;
    cv::calcHist(&gray, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange);

    cv::Mat cumulativeHist = hist.clone();
    for (int i = 1; i < histSize; i++)
    {
        cumulativeHist.at<float>(i) += cumulativeHist.at<float>(i - 1);
    }

    cumulativeHist /= cumulativeHist.at<float>(histSize - 1);

    output = input.clone();
    for (int y = 0; y < input.rows; y++)
    {
        for (int x = 0; x < input.cols; x++)
        {
            for (int c = 0; c < 3; c++)
            {
                output.at<cv::Vec3b>(y, x)[c] = static_cast<uchar>(cumulativeHist.at<float>(input.at<cv::Vec3b>(y, x)[c]) * 255.0f);
            }
        }
    }
}