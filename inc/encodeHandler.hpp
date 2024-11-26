#ifndef JIKIMZON_ENCODEHANDLER_H
#define JIKIMZON_ENCODEHANDLER_H

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/imgutils.h>
    #include <libswscale/swscale.h>

    // int avcodec_open2(AVCodecContext *avctx, AVCodec *codec, AVDictionary **options);
}

#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>

#define OUT

class EncodeHandler {
public:
    EncodeHandler(int width, int height, int bitrate, int fps);
    ~EncodeHandler();
    
    void EncodeFrame(cv::Mat& frame, std::vector<uint8_t>& OUT encodedFrame);
private:
    void initEncoder();

    SwsContext* mSwsContext;
    AVCodecContext* mCodecContext;

    int mWidth;
    int mHeight;
    int mBitrate;
    int mFps;

    std::ofstream mFS;
};


#endif