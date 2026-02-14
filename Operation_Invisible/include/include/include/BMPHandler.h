#ifndef BMPHANDLER_H
#define BMPHANDLER_H

#include <cstdint>
#include <vector>
#include <string>

// Force 1-byte alignment so structs match binary file layout
#pragma pack(push, 1)

struct BMPFileHeader {
    uint16_t fileType{0x4D42}; // File signature "BM"
    uint32_t fileSize{0};      // File size in bytes
    uint16_t reserved1{0};     // Reserved, always 0
    uint16_t reserved2{0};     // Reserved, always 0
    uint32_t offsetData{0};    // Start position of pixel data
};

struct BMPInfoHeader {
    uint32_t size{0};          // Size of this header
    int32_t  width{0};         // width of image in pixels
    int32_t  height{0};        // height of image in pixels
    uint16_t planes{1};        // number of color planes
    uint16_t bitCount{0};      // bits per pixel
    uint32_t compression{0};   // compression method (0 for uncompressed)
    uint32_t sizeImage{0};     // image size
    int32_t  xPixelsPerMeter{0};
    int32_t  yPixelsPerMeter{0};
    uint32_t colorsUsed{0};
    uint32_t colorsImportant{0};
};

#pragma pack(pop)

struct BMPImage {
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    std::vector<uint8_t> pixelData; // Raw pixel bytes
};

#endif // BMPHANDLER_H