#include "../include/StegoCore.h"
#include <fstream>
#include <iostream>
#include <cstring>

// ---------------------------------------------------------
// FILE I/O OPERATIONS
// ---------------------------------------------------------

bool StegoCore::loadBMP(const std::string& filename, BMPImage& image) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }

    // Read headers directly into structs
    file.read(reinterpret_cast<char*>(&image.fileHeader), sizeof(BMPFileHeader));
    file.read(reinterpret_cast<char*>(&image.infoHeader), sizeof(BMPInfoHeader));

    // Verify it's a BMP (Signature "BM")
    if (image.fileHeader.fileType != 0x4D42) {
        std::cerr << "Error: Not a valid BMP file." << std::endl;
        return false;
    }

    // Move to pixel data start position
    file.seekg(image.fileHeader.offsetData, std::ios::beg);

    // Calculate size of pixel data
    // Note: fileSize includes headers, so we subtract offsetData? 
    // Safer to read until EOF or rely on infoHeader.sizeImage if valid.
    // For simplicity, let's read the rest of the file.
    int dataSize = image.fileHeader.fileSize - image.fileHeader.offsetData;
    
    image.pixelData.resize(dataSize);
    file.read(reinterpret_cast<char*>(image.pixelData.data()), dataSize);

    file.close();
    return true;
}

bool StegoCore::saveBMP(const std::string& filename, const BMPImage& image) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) return false;

    // Write headers
    file.write(reinterpret_cast<const char*>(&image.fileHeader), sizeof(BMPFileHeader));
    file.write(reinterpret_cast<const char*>(&image.infoHeader), sizeof(BMPInfoHeader));

    // Write pixel data
    file.write(reinterpret_cast<const char*>(image.pixelData.data()), image.pixelData.size());
    
    file.close();
    return true;
}

// ---------------------------------------------------------
// BITWISE MANIPULATION (THE CORE LOGIC)
// ---------------------------------------------------------

// Helper: Hides 8 bits of 'byteToHide' into 8 bytes of 'dataPtr'
void StegoCore::embedByte(uint8_t* dataPtr, uint8_t byteToHide) {
    for (int i = 0; i < 8; i++) {
        // 1. Get the bit at position i (0 or 1)
        int bit = (byteToHide >> i) & 1;

        // 2. Clear the LSB of the image byte (Using AND mask 11111110)
        dataPtr[i] &= 0xFE;

        // 3. Set the LSB to our secret bit (Using OR)
        dataPtr[i] |= bit;
    }
}

// Helper: Reconstructs a byte from 8 bytes of 'dataPtr'
uint8_t StegoCore::extractByte(const uint8_t* dataPtr) {
    uint8_t extractedByte = 0;
    for (int i = 0; i < 8; i++) {
        // 1. Get the LSB from the image byte
        int bit = dataPtr[i] & 1;

        // 2. Shift it to the correct position and add to result
        extractedByte |= (bit << i);
    }
    return extractedByte;
}

bool StegoCore::embedMessage(BMPImage& image, const std::string& message) {
    uint32_t len = message.length();
    
    // Check capacity: We need 8 bytes of image for every 1 byte of secret
    // Plus 32 bytes (4 * 8) to store the length integer itself
    size_t requiredBytes = (sizeof(uint32_t) + len) * 8;
    
    if (requiredBytes > image.pixelData.size()) {
        std::cerr << "Error: Image too small to hold this message." << std::endl;
        return false;
    }

    int cursor = 0;

    // 1. Embed the Length (4 bytes)
    // We treat the uint32_t length as 4 separate chars
    for (int i = 0; i < 4; i++) {
        uint8_t byte = (len >> (i * 8)) & 0xFF;
        embedByte(&image.pixelData[cursor], byte);
        cursor += 8;
    }

    // 2. Embed the Message
    for (char c : message) {
        embedByte(&image.pixelData[cursor], c);
        cursor += 8;
    }

    return true;
}

std::string StegoCore::extractMessage(const BMPImage& image) {
    int cursor = 0;
    
    // 1. Extract Length
    uint32_t len = 0;
    for (int i = 0; i < 4; i++) {
        uint8_t byte = extractByte(&image.pixelData[cursor]);
        len |= (byte << (i * 8));
        cursor += 8;
    }

    // Sanity check on length
    if (len == 0 || (cursor + len * 8) > image.pixelData.size()) {
        return ""; // Invalid or empty
    }

    // 2. Extract Message
    std::string message;
    message.resize(len);
    for (uint32_t i = 0; i < len; i++) {
        message[i] = extractByte(&image.pixelData[cursor]);
        cursor += 8;
    }

    return message;
}