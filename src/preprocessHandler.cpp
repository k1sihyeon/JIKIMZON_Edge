#include "preprocessHandler.hpp"

#include <thread>

PreprocessHandler::PreprocessHandler(cv::Mat& src, cv::Mat& dest) : mSrc(src), mDest(dest)
{
    cv::Mat gray;
    cv::cvtColor(mSrc, gray, cv::COLOR_BGR2GRAY);

    int histSize = 256;
    float range[] = {0, 256};
    const float* histRange = {range};
    cv::Mat hist;
    cv::calcHist(&gray, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange);

    mCumulativeHist = hist.clone();

    for (int i = 1; i < histSize; i++)
    {
        mCumulativeHist.at<float>(i) += mCumulativeHist.at<float>(i - 1);
    }

    mCumulativeHist /= mCumulativeHist.at<float>(histSize - 1);
}

void PreprocessHandler::Threading()
{
    const int numThread = 4;
    const int rowsPerThread = mSrc.rows / numThread;

    std::vector<std::thread> worker;

    for (int i = 0; i < numThread; i++)
    {
        int start = i * rowsPerThread;
        int end = (i == numThread - 1) ? mSrc.rows : (i + 1) * rowsPerThread;

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

void PreprocessHandler::exposure(const int start, const int end)
{
    for (int y = start; y < end; y++)
    {
        for (int x = 0; x < mSrc.cols; x++)
        {
            for (int c = 0; c < 3; c++)
            {
                mDest.at<cv::Vec3b>(y, x)[c] = static_cast<uchar>(mCumulativeHist.at<float>(mSrc.at<cv::Vec3b>(y, x)[c]) * 255.0f);
            }
        }
    }
}