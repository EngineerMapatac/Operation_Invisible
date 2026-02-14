#ifndef STEGOCORE_H
#define STEGOCORE_H

#include "BMPHandler.h"
#include <string>
#include <vector>

class StegoCore {
public:
    // Reads a BMP file from disk into memory
    bool loadBMP(const std::string& filename, BMPImage& image);

    // Writes the modified BMP structure back to disk
    bool saveBMP(const std::string& filename, const BMPImage& image);

    // Hides the secret message into the image
    bool embedMessage(BMPImage& image, const std::string& message);

    // Extracts the hidden message from the image
    std::string extractMessage(const BMPImage& image);

private:
    // Helper: Modifies the LSB of a single byte
    void embedByte(uint8_t* dataPtr, uint8_t byteToHide);
    
    // Helper: Reads the LSB from a single byte
    uint8_t extractByte(const uint8_t* dataPtr);
};

#endif // STEGOCORE_H