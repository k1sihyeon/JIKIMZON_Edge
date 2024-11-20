#include "data.hpp"

std::string data::GetCurrentTime()
{
    auto now = std::chrono::system_clock::now();

    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    localtime_r(&currentTime, &tm);

    auto duration = now.time_since_epoch();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000;

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y%m%d_%H%M%S");
    oss << "." << std::setfill('0') << std::setw(3) << milliseconds.count();

	// format: YYYYMMDD_HHMMSS.sss

    return oss.str();
}

std::string data::PadFixedLength(std::string& str, int length = data::CLASSNAME_SIZE)
{
    if (str.length() > length)
    {
        return str.substr(0, length);
    }
    else
    {
        return str + std::string(length - str.length(), ' ');
    }
}

void data::SerializeFrame(std::string timestamp, unsigned char *iv, uint8_t *encryptedFrame, std::vector<uint8_t>& buffer)
{
    buffer.clear();

    // timestamp
    buffer.insert(buffer.end(), timestamp.begin(), timestamp.end());

    // initial vector for encryption
    buffer.insert(buffer.end(), iv, iv + data::IV_SIZE);

    // encryptedFrame
    buffer.insert(buffer.end(), encryptedFrame, encryptedFrame + data::FRAME_SIZE);
}

void data::SerializeFrame(data::Frame& frameData, std::vector<uint8_t>& buffer)
{
    return SerializeFrame(frameData.timestamp, frameData.iv, frameData.encryptedFrame, buffer);
}

void data::DeserializeFrame(std::vector<uint8_t>& buffer, data::Frame& frameData)
{
    size_t offset = data::FRAME_SIZE;

    // timestamp
    frameData.timestamp = std::string(buffer.begin(), buffer.begin() + offset);

    // initial vector for encryption
    std::copy(buffer.begin() + offset, buffer.begin() + offset + data::IV_SIZE, frameData.iv);
    offset += data::IV_SIZE;

    // encryptedFrame
    std::copy(buffer.begin() + offset, buffer.end(), frameData.encryptedFrame);
}

void data::SerializeDetection(data::Detection& detection, std::vector<uint8_t>& buffer)
{
    buffer.clear();

    // timestamp
    buffer.insert(buffer.end(), detection.timeStamp.begin(), detection.timeStamp.end());

    // classId
    buffer.insert(buffer.end(), (uint8_t*)&detection.classId, (uint8_t*)&detection.classId + sizeof(int));

    // className
    std::string padClassName = PadFixedLength(detection.className);
    buffer.insert(buffer.end(), padClassName.begin(), padClassName.end());

    // confidence
    buffer.insert(buffer.end(), (uint8_t*)&detection.confidence, (uint8_t*)&detection.confidence + sizeof(float));

    // box
    buffer.insert(buffer.end(), (uint8_t*)&detection.box.x, (uint8_t*)&detection.box.x + sizeof(int));
    buffer.insert(buffer.end(), (uint8_t*)&detection.box.y, (uint8_t*)&detection.box.y + sizeof(int));
    buffer.insert(buffer.end(), (uint8_t*)&detection.box.width, (uint8_t*)&detection.box.width + sizeof(int));
    buffer.insert(buffer.end(), (uint8_t*)&detection.box.height, (uint8_t*)&detection.box.height + sizeof(int));
}

void data::DeserializeDetection(std::vector<uint8_t>& buffer, data::Detection& detection)
{
    size_t offset = 0;

    detection.timeStamp = std::string(buffer.begin(), buffer.begin() + data::TIMESTAMP_SIZE);
    offset += data::TIMESTAMP_SIZE;

    detection.classId = *(int*)&buffer[offset];
    offset += sizeof(int);

    detection.className = std::string(buffer.begin() + offset, buffer.begin() + offset + data::CLASSNAME_SIZE);
    offset += data::CLASSNAME_SIZE;

    detection.confidence = *(float*)&buffer[offset];
    offset += sizeof(float);

    detection.box.x = *(int*)&buffer[offset];
    offset += sizeof(int);

    detection.box.y = *(int*)&buffer[offset];
    offset += sizeof(int);

    detection.box.width = *(int*)&buffer[offset];
    offset += sizeof(int);

    detection.box.height = *(int*)&buffer[offset];
    offset += sizeof(int);
}