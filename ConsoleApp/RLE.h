#pragma once

#include <string>

using namespace std;

void compressTextRLE(string& text, string& cText) {
    cText = text[0];
    char prevChar = text[0];
    int count = 1;
    for (int i = 1; i < text.size(); i++) {
        if (text[i] == prevChar) count++;
        else {
            prevChar = text[i];
            if (count > 1)
                cText += to_string(count);
            cText += prevChar;
            count = 1;
        }
    }
    if (count > 1)
        cText += to_string(count);
}

void decompressTextRLE(string& text, string& dText) {
    dText = text[0];
    for (int i = 1; i < text.size(); i++) {
        if (dText[i] <= 57 and dText[i] >= 50) {
            int count = dText[i] - 1;
            while (count--) {
                dText += text[i - 1];
            }
        }
        else {
            dText += text[i];
        }
    }
}