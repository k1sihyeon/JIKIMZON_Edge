#ifndef JIKIMZON_HEADER_H
#define JIKIMZON_HEADER_H

#include <cstdint>
#include <vector>
#include <string>

#include "ISerializeInterface.hpp"

namespace frame
{
	/*
    ----------------------------------------------------------------
    |                      frameId (4 bytes)                       |
    ----------------------------------------------------------------
    |                           bodySize (4)                       |
    ----------------------------------------------------------------
    |          imageWidth (2)       |        imageHeight (2)       |
    ----------------------------------------------------------------
    | imageFormat (1)  |                  Padding (3)              |
    ----------------------------------------------------------------
    |                         timestamp (4)                        |
    ----------------------------------------------------------------
    |                         timestamp (4)                        |
    ----------------------------------------------------------------
    |                         timestamp (4)                        |
    ----------------------------------------------------------------
    |                         timestamp (4)                        |
    ----------------------------------------------------------------
    |                         timestamp (3)       |  padding (1)   |
    ----------------------------------------------------------------
	| GopStartFlag (1) | GopSize (1) |       padding (2)           |
    ----------------------------------------------------------------
    */

    struct HeaderStruct
    {
        uint32_t frameId;   // 4 bytes
        uint32_t bodySize;  // 4 bytes

        uint16_t imageWidth;   // 2 bytes
        uint16_t imageHeight;  // 2 bytes
        uint8_t imageFormat;   // 1 byte
        uint8_t padding1[3];   // 3 bytes (for alignment)

        char timestamp[19];   // 19 bytes
        uint8_t padding2[1];  // 1 byte (for alignment)

        uint8_t gopStartFlag;   // 1 byte
		uint8_t gopSize;        // 1 byte
		uint8_t padding3[2];    // 2 bytes (for alignment)
    };

    namespace GopStartFlag
    {
        enum : uint8_t
        {
            START = 0x01,
            END = 0x00
        };
    }

    class Header : virtual public common::ISerializeInterface
    {
    public:
        Header() = default;
        Header(HeaderStruct& header);
        ~Header() = default;

        std::vector<uint8_t> Serialize() const override;
        void Serialize(std::vector<uint8_t>& OUT buffer) const override;
        void Deserialize(std::vector<uint8_t>& buffer) override;

        uint32_t GetFrameId() const;
        uint32_t GetBodySize() const;
        uint16_t GetImageWidth() const;
        uint16_t GetImageHeight() const;
        uint8_t GetImageFormat() const;
        std::string GetTimestamp() const;
        uint8_t GetGopStartFlag() const;
        uint8_t GetGopSize() const;

        void SetHeader(HeaderStruct& header);
    
    private:
        HeaderStruct mHeader;
    };
}

#endif // JIKIMZON_HEADER_H