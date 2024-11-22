#include "encodeHandler.hpp"

EncodeHandler::EncodeHandler(int width, int height, int bitrate, int fps)
                    : mCodecContext(nullptr), mWidth(width), mHeight(height), mBitrate(bitrate), mFps(fps)
{
    initEncoder();
    mFS.open("output.h264", std::ios::binary);
    if (!mFS.is_open())
    {
        std::cerr << "Cannot open output file" << std::endl;
    }
}

EncodeHandler::~EncodeHandler()
{
    if (mCodecContext)
    {
        avcodec_free_context(&mCodecContext);
    }

    sws_freeContext(mSwsContext);

    if (mFS.is_open())
    {
        mFS.close();
    }
}

void EncodeHandler::initEncoder()
{
    std::cout << "THIS IS INITENCODER" << std::endl;
    const AVCodec* avCodec = avcodec_find_encoder(AV_CODEC_ID_H264);

    if (!avCodec)
    {
        perror("Codec not found");
    }

    mCodecContext = avcodec_alloc_context3(avCodec);
    if (!mCodecContext)
    {
        perror("Cannot allocate codec context");
    }

    mCodecContext->bit_rate = 1000000;
    mCodecContext->width = mWidth;
    mCodecContext->height = mHeight;
    mCodecContext->time_base = (AVRational){1, mFps};
    mCodecContext->framerate = (AVRational){mFps, 1};
    mCodecContext->gop_size = 10;
    mCodecContext->max_b_frames = 0;
    mCodecContext->pix_fmt = AV_PIX_FMT_YUV420P;

    AVDictionary *param = nullptr;
    av_dict_set(&param, "preset", "ultrafast", 0);
    av_dict_set(&param, "tune", "zerolatency", 0);

    if (avcodec_open2(mCodecContext, avCodec, &param) < 0)
    {
        perror("Cannot open codec");
    }

    mSwsContext = sws_getContext(mWidth, mHeight, AV_PIX_FMT_BGR24, mWidth, mHeight, AV_PIX_FMT_YUV420P, SWS_BICUBIC, nullptr, nullptr, nullptr);
}

void EncodeHandler::EncodeFrame(cv::Mat& frame, std::vector<uint8_t>& encodedFrame)
{
    AVFrame* avFrame = av_frame_alloc();
    if (!avFrame)
    {
        std::cerr << "cannot allocate video frame" << std::endl;
    }

    avFrame->format = mCodecContext->pix_fmt;
    avFrame->width = mCodecContext->width;
    avFrame->height = mCodecContext->height;
    av_image_alloc(avFrame->data, avFrame->linesize, mCodecContext->width, mCodecContext->height, mCodecContext->pix_fmt, 32);

    int ret = av_image_alloc(avFrame->data, avFrame->linesize, mCodecContext->width, mCodecContext->height, mCodecContext->pix_fmt, 32);
    if (ret < 0)
    {
        std::cerr << "cannot allocate frame buffer" << std::endl;
    }

    const uint8_t* inData[1] = {frame.data};
    int inLineSize[1] = {static_cast<int>(frame.step)};
    sws_scale(mSwsContext, inData, inLineSize, 0, mHeight, avFrame->data, avFrame->linesize);

    AVPacket pkt;
    av_init_packet(&pkt);
    pkt.data = nullptr;
    pkt.size = 0;

    ret = avcodec_send_frame(mCodecContext, avFrame);
    if (ret < 0)
    {
        perror("Error: send frame to encoder");
    }

    ret = avcodec_receive_packet(mCodecContext, &pkt);
    if (ret == 0)
    {
        encodedFrame.assign(pkt.data, pkt.data + pkt.size);
        // code for save the h.264 for testing
        // mFS.write(reinterpret_cast<const char*>(pkt.data), pkt.size);
        av_packet_unref(&pkt);
    }

    av_freep(&avFrame->data[0]);
    av_frame_free(&avFrame);
}