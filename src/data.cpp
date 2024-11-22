#include "data.hpp"

void data::SerializeFrameHeader(data::FrameHeader& frameHeader, std::vector<uint8_t>& buffer)
{
    buffer.clear();
    buffer.resize(sizeof(data::FrameHeader));
    std::memcpy(buffer.data(), &frameHeader, sizeof(data::FrameHeader));
}

void data::DeserializeFrameHeader(std::vector<uint8_t>& buffer, data::FrameHeader& frameHeader)
{
    std::memcpy(&frameHeader, buffer.data(), sizeof(data::FrameHeader));
}
