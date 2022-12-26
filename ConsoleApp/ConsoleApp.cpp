// ConsoleApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>

#include "LZ77.h"
#include "LZ78.h"
#include "Deflate.h"
#include "RLE.h"
#include "Huffman.h"

using namespace std;

enum Algorithm : unsigned char{
    RLE, LZ77, LZ78, HUFFMAN, DEFLATE
};

void compressText(string& text, string& cText, string& encodingFilePath, Algorithm algo = RLE) {
    switch (algo) {
    case RLE: compressTextRLE(text, cText); break;
    case LZ77: compressTextLZ77(text, cText); break;
    case LZ78: compressTextLZ78(text, cText); break;
    case HUFFMAN: compressTextHUFFMAN(text, cText, encodingFilePath); break;
    case DEFLATE: compressTextDEFLATE(text, cText); break;
    }
}

void decompressText(string& text, string& cText, string& encodingFilePath, Algorithm algo = RLE) {
    switch (algo) {
    case RLE: decompressTextRLE(text, cText); break;
    case LZ77: decompressTextLZ77(text, cText); break;
    case LZ78: decompressTextLZ78(text, cText); break;
    case HUFFMAN: decompressTextHUFFMAN(text, cText, encodingFilePath); break;
    case DEFLATE: decompressTextDEFLATE(text, cText); break;
    }
}

int main(){
    ifstream file;
    string text;

    file.open("C:/Users/Lenovo/OneDrive/Desktop/Software/FileCompression/ConsoleApp/TextFile.txt", ios::in);
    if (file.is_open()) {
        getline(file, text);
        file.close();
    }


    string encodingFilePath = "C:/Users/Lenovo/OneDrive/Desktop/Software/FileCompression/ConsoleApp/HuffmanEncodingFile.txt";
    string compressedText;
    compressText(text, compressedText, encodingFilePath, HUFFMAN);

    ofstream out("C:/Users/Lenovo/OneDrive/Desktop/Software/FileCompression/ConsoleApp/CompressedTextFile.txt");
    out << compressedText;

    file.open("C:/Users/Lenovo/OneDrive/Desktop/Software/FileCompression/ConsoleApp/CompressedTextFile.txt", ios::in);
    if (file.is_open()) {
        getline(file, text);
        file.close();
    }

    string decompressedText;
    decompressText(compressedText, decompressedText, encodingFilePath, HUFFMAN);
}