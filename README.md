
```
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

```

### 2. The LSB Injection Algorithm

The injection logic iterates through the secret message, effectively "serializing" characters into bits and distributing them across the image's pixel array.

**Visual Representation:**
`[Image Byte: 1111 1110]` + `[Secret Bit: 1]` = `[Modified Byte: 1111 1111]`

```cpp
// From src/StegoCore.cpp
void StegoCore::embedByte(uint8_t* pixelData, char secretChar, int offset) {
    // Iterate through all 8 bits of the character
    for (int i = 0; i < 8; i++) {
        
        // Step 1: Isolate the i-th bit of the secret character
        // Example: If char is 'A' (01000001), get the bit at position i
        int bitToHide = (secretChar >> i) & 1;

        // Step 2: Clear the LSB of the current image byte
        // Using bitwise AND with 0xFE (Mask: 1111 1110)
        pixelData[offset + i] &= 0xFE; 

        // Step 3: Insert the secret bit
        // Using bitwise OR to place the bit into the LSB slot
        pixelData[offset + i] |= bitToHide;
    }
}

```

---

## 🚀 Features

* **Zero External Dependencies:** Built entirely with the C++ Standard Library (`<fstream>`, `<vector>`, `<iostream>`).
* **Low-Level Memory Management:** Demonstrates manual buffer handling and pointer arithmetic.
* **Binary File I/O:** Directly reads and writes binary streams without abstraction layers.
* **Custom Payload Header:** Embeds message length metadata to ensure precise extraction.

---

## 🛠️ Build Instructions

### Prerequisites

* **Compiler:** G++ (MinGW for Windows, GCC for Linux/macOS) or Clang.
* **Standard:** C++17 or higher.

### Compiling

Clone the repository and compile the source code:

```bash
git clone [https://github.com/your-username/Operation_Invisible.git](https://github.com/your-username/Operation_Invisible.git)
cd Operation_Invisible

# Compile using g++
g++ -o invisible src/main.cpp src/StegoCore.cpp src/BMPHandler.cpp -I include/

```

---

## 💻 Usage Guide

### Encoding (Hiding Data)

Embed a secret text string into a carrier image.

```bash
./invisible encode --source assets/cover.bmp --message "The Flame is safe." --output assets/stego.bmp

```

### Decoding (Retrieving Data)

Extract the hidden message from a modified image.

```bash
./invisible decode --source assets/stego.bmp

```

---

## 📂 Project Structure

```text
Operation_Invisible/
├── src/
│   ├── main.cpp          # CLI entry point & argument parsing
│   ├── StegoCore.cpp     # Bitwise injection/extraction logic
│   └── BMPHandler.cpp    # Binary file I/O implementation
├── include/
│   ├── StegoCore.h       # Class headers
│   └── BMPHandler.h      # Struct definitions for BMP format
├── assets/
│   ├── cover.bmp         # Original carrier image
│   └── output.bmp        # Resulting image with hidden data
└── README.md

```

---

## 🔧 Future Roadmap

* [ ] **Encryption Layer:** Implement XOR or AES encryption on the payload before embedding.
* [ ] **WAV Support:** Extend functionality to hide data in audio files (LSB of audio samples).
* [ ] **Steganalysis:** Add a utility to calculate the Signal-to-Noise Ratio (SNR) of the modified image.

---

## 👤 Author

**John Ramil Mapatac** *3rd Year Computer Engineering Student | Airbnb Intern Team Lead*

This project was created to demonstrate proficiency in **System Architecture**, **C++ Memory Management**, and **Data Structures**.

## 📜 License

This project is licensed under the MIT License - see the [LICENSE]([https://www.apache.org/licenses/LICENSE-2.0]) file for details.

```

```
