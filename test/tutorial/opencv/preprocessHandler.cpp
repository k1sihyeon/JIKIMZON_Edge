#include "preprocessHandler.hpp"

#include <thread>

PreprocessHandler::PreprocessHandler(cv::Mat& src, cv::Mat& dest) : src(src), dest(dest)
{
    cv::Mat gray;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

    int histSize = 256;
    float range[] = {0, 256};
    const float* histRange = {range};
    cv::Mat hist;
    cv::calcHist(&gray, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange);

    cumulativeHist = hist.clone();

    for (int i = 1; i < histSize; i++)
    {
        cumulativeHist.at<float>(i) += cumulativeHist.at<float>(i - 1);
    }

    cumulativeHist /= cumulativeHist.at<float>(histSize - 1);
}

void PreprocessHandler::Threading()
{
    const int numThread = 4;
    const int rowsPerThread = src.rows / numThread;

    std::vector<std::thread> worker;

    for (int i = 0; i < numThread; i++)
    {
        int start = i * rowsPerThread;
        int end = (i == numThread - 1) ? src.rows : (i + 1) * rowsPerThread;
        // std::cout << "thread(" << start << ", " << end << "): " << i << std::endl;

        worker.emplace_back(std::thread(&PreprocessHandler::Exposure, this, start, end));
    }

    for (auto& work: worker)
    {
        if (work.joinable())
        {
            work.join();
        }
    }
}

void PreprocessHandler::Exposure(const int start, const int end)
{
    dest = src.clone();
    for (int y = start; y < end; y++)
    {
        for (int x = 0; x < src.cols; x++)
        {
            std::cout << '(' << y << ", " << x << ')' << std::endl;
            for (int c = 0; c < 3; c++)
            {
                dest.at<cv::Vec3b>(y, x)[c] = static_cast<uchar>(cumulativeHist.at<float>(src.at<cv::Vec3b>(y, x)[c]) * 255.0f);
            }
        }
    }
}