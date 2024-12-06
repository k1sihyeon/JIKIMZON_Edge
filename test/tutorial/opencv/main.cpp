#include "preprocessHandler.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
    cv::Mat input;
    cv::Mat output;

    input = cv::imread(argv[1]);
    if (input.empty())
    {
        std::cerr << "Error: imread" << std::endl;
    }
    
    output = input.clone();

    cv::imshow("Original", input);

    PreprocessHandler* ph = new PreprocessHandler(input, output);

    auto start = std::chrono::high_resolution_clock::now();
    ph->Exposure(0, input.rows);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "norm: " << duration << "ms" << std::endl;

    cv::imshow("norm", output);

    start = std::chrono::high_resolution_clock::now();
    ph->Threading();
    end = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "thre: " << duration << "ms" << std::endl;

    cv::imshow("thre", output);
    cv::waitKey(0);

    return 0;
}