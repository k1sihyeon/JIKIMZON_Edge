#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <numeric>

double PSNR(cv::Mat& input, cv::Mat& ouput);

int main(int argc, char* argv[])
{
    cv::VideoCapture inputCap(argv[1]);
    cv::VideoCapture outputCap(argv[2]);
    
    cv::Mat input;
    cv::Mat output;
    std::vector<double> psnrs;
    
    while (true) 
    {
        inputCap >> input;
        outputCap >> output;

        if (input.empty() || output.empty())
        {
            break;
        }
        
        psnrs.push_back(PSNR(input, output));
    }
    
    std::cout << "==============================" << std::endl;
    std::cout << "   PSNR Report" << std::endl;
    std::cout << "==============================" << std::endl;
    std::cout << "   Minimum PSNR: " << *std::min_element(psnrs.begin(), psnrs.end()) << " dB" << std::endl;
    std::cout << "   Maximum PSNR: " << *std::max_element(psnrs.begin(), psnrs.end()) << " dB" << std::endl;
    std::cout << "   Average PSNR: " << std::accumulate(psnrs.begin(), psnrs.end(), 0.0) / psnrs.size() << " dB" << std::endl;
    std::cout << "==============================" << std::endl;
    
    return 0; 
}

double PSNR(cv::Mat& input, cv::Mat& output)
{
    if (input.size() != output.size())
    {
        std::cerr << "Images are not same size" << std::endl;
    }

    if (input.type() != output.type())
    {
        std::cerr << "Images are not same type" << std::endl;
    }
    
    // mse 계산
    cv::Mat diff;
    cv::absdiff(input, output, diff);
    diff.convertTo(diff, CV_32F);
    diff = diff.mul(diff);

    double mse = cv::sum(diff)[0] / (input.total() * input.channels());

    // 두 이미지가 동일
    if (mse == 0)
    {
        std::cout << "Images are same" << std::endl;
        return 0.0;
    }

    // PSNR 계산
    double psnr = 10.0 * std::log10((255 * 255) / mse);

    std::cout << "mse: " << mse << std::endl;
    std::cout << "psnr: " << psnr << std::endl;

    return psnr;
}
