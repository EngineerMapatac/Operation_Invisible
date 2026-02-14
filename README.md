**Operation_Invisible**

Operation_Invisible is a high-performance, dependency-free C++ steganography engine. It conceals data payloads within uncompressed BMP images by directly manipulating the Least Significant Bit (LSB) of raw pixel data.

This project demonstrates core Computer Engineering concepts: Memory Management, File Structure Parsing, and Binary Arithmetic.

🧠 Core Logic & Implementation
Unlike high-level scripts that abstract away the data, this engine manually handles memory buffers and binary streams.

1. Manual BMP Header Parsing
To read a bitmap without external libraries, we must strictly define the file structure. We use #pragma pack to prevent the compiler from adding padding bytes, ensuring our struct aligns perfectly with the binary file format.

