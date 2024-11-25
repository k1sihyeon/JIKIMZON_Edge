#include "preprocessHandler.hpp"

int main()
{
    cv::Mat input = cv::imread("./lenna.jpg");
    if (input.empty())
    {
        std::cerr << "Error: imread" << std::endl;
    }

    cv::Mat output;
    adaptiveExposureControl(input, output);

    cv::imshow("Original", input);
    cv::imshow("Adaptive Exposure Control", output);
    cv::waitKey(0);

    return 0;
}