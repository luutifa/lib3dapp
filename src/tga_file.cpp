#include "tga_file.hpp"
#include "define.hpp"
#include <iostream>
#include <cstdlib>
#include <fstream>

void errorPrint(std::string filename) {
    std::cout << "loadTGAFile(" << filename << "): Failed to load TGA file\n";
}

RgbaImage loadTGAFile(std::string filename) {
    std::ifstream file;
    unsigned char header[6];
    unsigned char* imageData;
    unsigned int imageSize;
    unsigned int bpp;
    unsigned int bytespp;
    unsigned int width;
    unsigned int height;
    unsigned int pixelCount;
        
    const unsigned char uTGAcompare[12]={0,0,2,0,0,0,0,0,0,0,0,0};
    const unsigned char cTGAcompare[12]={0,0,10,0,0,0,0,0,0,0,0,0};
    unsigned char formatHeader[12];

    file.open(filename.c_str());
    if (!file.is_open()) {
        std::cout << "loadTGAFile(" << filename << "): No such file or directory\n";
        exit(ERR_TGA_FILE);
    }

    file.read((char*)formatHeader, sizeof(formatHeader));
    if (file.eof() || file.fail()) {
        errorPrint(filename);
        exit(ERR_TGA_FILE);
    }

    file.read((char*)header, sizeof(header));
    if (file.eof() || file.fail()) {
        errorPrint(filename);
        exit(ERR_TGA_FILE);
    }

    width = header[1] * 256 + header[0];
    height = header[3] * 256 + header[2];
    bpp = header[4];

    if ((width <= 0) || (height <= 0) || ((bpp != 24) && (bpp != 32))) {
        errorPrint(filename);
        exit(ERR_TGA_FILE);
    }
    
    pixelCount = width * height;
    bytespp = bpp/8;
    imageSize = bytespp * pixelCount;

    imageData = (unsigned char*)malloc(imageSize);
    if (imageData == NULL) {
        errorPrint(filename);
        exit(ERR_TGA_FILE);
    }

    if (memcmp(uTGAcompare, formatHeader, sizeof(formatHeader))==0) {   // If TGA uncompressed
        file.read((char*)imageData, imageSize);
        
        if (file.fail()) {
            errorPrint(filename);
            exit(ERR_TGA_FILE);
        }

        for (unsigned int cswap = 0; cswap < imageSize; cswap+=bytespp) {
            imageData[cswap] ^= imageData[cswap+2] ^=
            imageData[cswap] ^= imageData[cswap+2];
        }
    } else if (memcmp(cTGAcompare, formatHeader, sizeof(formatHeader))==0) { // If TGA compressed
        unsigned int curPixel = 0;
        unsigned int curByte = 0;
        unsigned char* colBuf = (unsigned char*)malloc(bytespp);
    
        do {
            unsigned char chunkheader = 0;
    
            file.read((char*)&chunkheader, sizeof(unsigned char));
            if (file.fail()) {
                errorPrint(filename);
                exit(ERR_TGA_FILE);
            }
    
            if (chunkheader < 128) {
                chunkheader++;
    
                for (unsigned short counter = 0; counter < chunkheader; counter++) {
                    file.read((char*)colBuf, bytespp);
                    if (file.fail()) {
                        errorPrint(filename);
                        exit(ERR_TGA_FILE);
                    }
    
                    imageData[curByte] = colBuf[2];
                    imageData[curByte+1] = colBuf[1];
                    imageData[curByte+2] = colBuf[0];
                    if (bytespp == 4)
                        imageData[curByte+3] = colBuf[3];
                    curByte += bytespp;
                    curPixel++;
                }
            } else {
                chunkheader -= 127;
    
                file.read((char*)colBuf, bytespp);
                if (file.fail()) {
                    errorPrint(filename);
                    exit(ERR_TGA_FILE);
                }
    
                for (unsigned short counter = 0; counter < chunkheader; counter++) {
                    imageData[curByte] = colBuf[2];
                    imageData[curByte+1] = colBuf[1];
                    imageData[curByte+2] = colBuf[0];
                    if (bytespp == 4)
                        imageData[curByte+3] = colBuf[3];
    
                    curByte += bytespp;
                    curPixel++;
                }
            }
        } while (curPixel < pixelCount);
        delete colBuf;
    } else {
        errorPrint(filename);
        exit(ERR_TGA_FILE);
    }
    file.close();
    
    RgbaImage image(width, height, true, (bpp==32));
    for (unsigned int i=0; i<imageSize; i++) {
        image.pushByte(imageData[i]);
    }
    
    return image;
}