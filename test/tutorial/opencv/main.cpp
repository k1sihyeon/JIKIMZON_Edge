#include "preprocessHandler.hpp"

#include <iostream>

int main()
{
    cv::Mat input;
    cv::Mat output;

    input = cv::imread("./lenna.jpg");
    if (input.empty())
    {
        std::cerr << "Error: imread" << std::endl;
    }

    cv::imshow("Original", input);

    // std::cout << "input: " << input.rows << ", " << input.cols << std::endl;

    PreprocessHandler* ph = new PreprocessHandler(input, output);

    auto start = std::chrono::high_resolution_clock::now();
    ph->Exposure(input.rows, input.cols);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "norm: " << duration << "ms" << std::endl;

    cv::imshow("norm", output);
    output.release();

    start = std::chrono::high_resolution_clock::now();
    ph->Threading();
    end = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "thre: " << duration << "ms" << std::endl;

    cv::imshow("thre", output);
    cv::waitKey(0);

    return 0;
}