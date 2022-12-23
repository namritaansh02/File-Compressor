// ConsoleApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>

using namespace std;

class HuffmanNode {
public:
    char ch;
    HuffmanNode* leftNode = nullptr;
    HuffmanNode* rightNode = nullptr;
    float prob = 0.;
    HuffmanNode() { this->ch = '~'; }
    HuffmanNode(char ch, float prob) { 
        this->ch = ch; 
        this->prob = prob; 
    }
    HuffmanNode(HuffmanNode* n1, HuffmanNode* n2, float prob) {
        this->ch = '~';
        this->leftNode = n1;
        this->rightNode = n2;
        this->prob = prob;
    }
};

HuffmanNode* joinNode(HuffmanNode* n1, HuffmanNode* n2) {
    HuffmanNode* jointNode = new HuffmanNode(n1, n2, n1->prob + n2->prob);
    return jointNode;
}

struct CompareHuffmanNode {
    bool operator()(const HuffmanNode* n1, const HuffmanNode* n2) { return n1->prob > n2->prob; } // to creat min
};

void encodingHuffman(priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareHuffmanNode>& nodeList, unordered_map<char, string>& encoding) {
    while (nodeList.size()>1) {
        HuffmanNode* n1, * n2;
        n1 = nodeList.top(); nodeList.pop();
        n2 = nodeList.top(); nodeList.pop();
        HuffmanNode* jointNode = joinNode(n1, n2);
        nodeList.push(jointNode);
    }

}

enum Algorithm : unsigned char{
    RLE, LZ77, LZ78, HUFFMAN, DEFLATE
};

void compressTextLZ77(string& text, string& cText) {

}

void compressTextLZ78(string& text, string& cText) {

}

void compressTextHUFFMAN(string& text, string& cText) {
    int totalLetters = text.size();
    
    priority_queue < HuffmanNode*, vector<HuffmanNode*>,  CompareHuffmanNode> nodeList;
    vector<float> probMap(256, 0);

    for (int i = 0; i < text.size(); i++)
        probMap[text[i]] += 1.0 / totalLetters;
   
    for (int i = 0; i < 256; i++) {
        if (probMap[i] > 0)
            nodeList.push(new HuffmanNode(i, probMap[i]));
    }

    unordered_map<char, string> encoding;
    encodingHuffman(nodeList, encoding);
}

void compressTextDEFLATE(string& text, string& cText) {

}

void compressTextRLE(string& text, string& cText) {
    cText = text[0];
    char prevChar = text[0];
    int count = 1;
    for (int i = 1; i < text.size(); i++) {
        if (text[i] == prevChar) count++;
        else {
            prevChar = text[i];
            if (count>1)
                cText += to_string(count);
            cText += prevChar;
            count = 1;
        }
    }
    if (count>1)
        cText += to_string(count);
}

void decompressTextRLE(string& text, string& dText) {
    dText = text[0];
    for (int i = 1; i < text.size(); i++) {
        if (dText[i] <= 57 and dText[i] >= 50) {
            int count = dText[i]-1;
            while (count--) {
                dText += text[i - 1];
            }
        }
        else {
            dText += text[i];
        }
    }
}

void compressText(string& text, string& cText, Algorithm algo = RLE) {
    switch (algo) {
    case RLE: compressTextRLE(text, cText); break;
    case LZ77: compressTextLZ77(text, cText); break;
    case LZ78: compressTextLZ78(text, cText); break;
    case HUFFMAN: compressTextHUFFMAN(text, cText); break;
    case DEFLATE: compressTextDEFLATE(text, cText); break;
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

    string compressedText;
    compressText(text, compressedText, HUFFMAN);
    cout << compressedText << "\n";
    ofstream out("C:/Users/Lenovo/OneDrive/Desktop/Software/FileCompression/ConsoleApp/CompressedTextFile.txt");
    out << compressedText;
}