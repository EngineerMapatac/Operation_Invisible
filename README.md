# Operation_Invisible

![Language](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg) ![Type](https://img.shields.io/badge/Implementation-Bitwise_Logic-orange.svg) ![License](https://img.shields.io/badge/License-MIT-green.svg)

> **A dependency-free C++ steganography tool for hiding data in BMP images using LSB manipulation and raw memory handling.**

**Operation_Invisible** is a high-performance steganography engine designed to conceal data payloads within uncompressed BMP images. Unlike high-level scripts that rely on external libraries (like OpenCV), this project implements **manual file parsing** and **direct memory buffer manipulation**, demonstrating core Computer Engineering competencies.

---

## 🧠 Core Logic & Implementation

The engine exploits the **Least Significant Bit (LSB)** of pixel data. By modifying the last bit of a byte, we can store binary information without altering the visual appearance of the image to the naked eye.

### 1. Manual BMP Header Parsing
To read a bitmap without dependencies, the software must strictly define the file structure. This implementation uses `#pragma pack` to prevent compiler padding, ensuring the structs align perfectly with the binary file format.

```cpp
// From include/BMPHandler.h
#pragma pack(push, 1) // Force 1-byte alignment for binary compatibility

struct BMPFileHeader {
    uint16_t fileType;      // File signature "BM" (0x4D42)
    uint32_t fileSize;      // Size of file in bytes
    uint16_t reserved1;     // Reserved (must be 0)
    uint16_t reserved2;     // Reserved (must be 0)
    uint32_t offsetData;    // Start position of pixel data
};

struct BMPInfoHeader {
    uint32_t size;          // Header size
    int32_t  width;         // Image width
    int32_t  height;        // Image height
    uint16_t planes;        // Number of color planes (1)
    uint16_t bitCount;      // Bits per pixel (24 for RGB)
    // ... compression and size fields omitted for brevity
};
#pragma pack(pop)
