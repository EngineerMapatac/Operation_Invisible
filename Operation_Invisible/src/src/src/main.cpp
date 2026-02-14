#include <iostream>
#include <string>
#include <cstring> // for strcmp
#include "../include/StegoCore.h"

void printUsage() {
    std::cout << "Operation_Invisible v1.0\n";
    std::cout << "Usage:\n";
    std::cout << "  Encode: ./invisible encode <input.bmp> <output.bmp> \"secret message\"\n";
    std::cout << "  Decode: ./invisible decode <stego_image.bmp>\n";
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printUsage();
        return 1;
    }

    StegoCore stego;
    BMPImage img;
    std::string mode = argv[1];

    if (mode == "encode") {
        if (argc < 5) {
            std::cout << "Error: Missing arguments for encoding.\n";
            printUsage();
            return 1;
        }

        std::string inputFile = argv[2];
        std::string outputFile = argv[3];
        std::string secretMsg = argv[4];

        std::cout << "[*] Loading carrier image: " << inputFile << "...\n";
        if (!stego.loadBMP(inputFile, img)) return 1;

        std::cout << "[*] Embedding payload (" << secretMsg.length() << " bytes)...\n";
        if (stego.embedMessage(img, secretMsg)) {
            if (stego.saveBMP(outputFile, img)) {
                std::cout << "[+] Success! Output saved to: " << outputFile << "\n";
            } else {
                std::cerr << "[-] Error saving file.\n";
            }
        }

    } else if (mode == "decode") {
        std::string inputFile = argv[2];

        std::cout << "[*] Analyzing stego-image: " << inputFile << "...\n";
        if (!stego.loadBMP(inputFile, img)) return 1;

        std::string secret = stego.extractMessage(img);
        if (secret.empty()) {
            std::cout << "[-] No valid message found or extraction failed.\n";
        } else {
            std::cout << "[+] DECODED PAYLOAD: " << secret << "\n";
        }

    } else {
        printUsage();
        return 1;
    }

    return 0;
}