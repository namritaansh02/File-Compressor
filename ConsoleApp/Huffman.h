#pragma once

#include <string>
#include <unordered_map>
#include <queue>

#include "utils.h"

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


void HuffmanDFS(HuffmanNode* root, unordered_map<char, string>& encoding, string preEncoding) {
    if (root->leftNode == nullptr and root->rightNode == nullptr) {
        encoding[root->ch] = preEncoding;
    }
    else {
        HuffmanDFS(root->leftNode, encoding, preEncoding + '0');
        HuffmanDFS(root->rightNode, encoding, preEncoding + '1');
    }
}

HuffmanNode* makeTreeHuffman(unordered_map<string, char>& decoding) {
    HuffmanNode* root = new HuffmanNode();

    for (auto key : decoding) {
        string encoding = key.first;
        HuffmanNode* currRoot = root;
        for (int i = 0; i < encoding.size(); i++) {
            if (encoding[i] == '0' && currRoot->leftNode == nullptr) {
                currRoot->leftNode = new HuffmanNode();
                currRoot = currRoot->leftNode;
            }
            else if (encoding[i] == '1' && currRoot->rightNode == nullptr) {
                currRoot->rightNode = new HuffmanNode();
                currRoot = currRoot->rightNode;
            }
            else if (encoding[i] == '0' && currRoot->leftNode != nullptr) {
                currRoot = currRoot->leftNode;
            }
            else if (encoding[i] == '1' && currRoot->rightNode != nullptr){
                currRoot = currRoot->rightNode;
            }
        }
        currRoot->ch = key.second;
    }

    return root;
}

void saveEncodingHuffman(unordered_map<char, string>& encoding, int zero_count, string& encodingFilePath) {
    ofstream encodingFile;
    encodingFile.open(encodingFilePath, ios::out);
    
    string to_write = to_string(zero_count) + "\n";
    for (auto i = encoding.begin(); i != encoding.end(); i++) {
        to_write += i->first;
        to_write += " " + i->second + "\n";
    }
    encodingFile << to_write;
}

void convertBinaryTextHuffman(string& text, string& dText, HuffmanNode* rootNode) {
    dText = ""; 
    HuffmanNode* currRoot = rootNode;

    for (auto ch:text) {
        if (ch == '1' && currRoot->rightNode != nullptr) {
            currRoot = currRoot->rightNode;
        }
        else if (ch == '0' && currRoot->leftNode != nullptr) {
            currRoot = currRoot->leftNode;
        }
        else if (currRoot->leftNode == nullptr && currRoot->rightNode == nullptr) {
            dText += currRoot->ch;
            if (ch == '0') currRoot = rootNode->leftNode; 
            else if (ch == '1') currRoot = rootNode->rightNode;
        }
    }
    dText += currRoot->ch;
}

void encodingHuffman(priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareHuffmanNode> nodeList, unordered_map<char, string>& encoding) {
    while (nodeList.size() > 1) {
        HuffmanNode* n1, * n2;
        n1 = nodeList.top(); nodeList.pop();
        n2 = nodeList.top(); nodeList.pop();
        HuffmanNode* jointNode = joinNode(n1, n2);
        nodeList.push(jointNode);
    }

    HuffmanNode* rootNode = nodeList.top(); nodeList.pop();
    HuffmanDFS(rootNode, encoding, "");
}

void readDecodingHuffman(string filepath, unordered_map<string, char>& decoding, int& zero_count) {
    ifstream decodingFile;
    string text = "", readText;
    decodingFile.open(filepath, ios::in);
    if (decodingFile.is_open()) {
        while (getline(decodingFile, readText)) text += readText + "\n";
        decodingFile.close();
    }
    zero_count = text[0] - 48;
    for (int i = 2; i < text.size();) {
        char currentChar = text[i];
        string encodingString = "";
        i += 2;
        while (text[i] != '\n') encodingString += text[i++];
        decoding[encodingString] = currentChar;
        i++;
    }
}

void compressTextHUFFMAN(string& text, string& cText, string& encodingFilePath) {
    int totalLetters = text.size();
    int initialSize = 8 * totalLetters;

    priority_queue < HuffmanNode*, vector<HuffmanNode*>, CompareHuffmanNode> nodeList;
    vector<float> probMap(128, 0);

    for (int i = 0; i < text.size(); i++)
        probMap[text[i]] += 1.0 / totalLetters;

    for (int i = 0; i < 128; i++) {
        if (probMap[i] > 0)
            nodeList.push(new HuffmanNode(i, probMap[i]));
    }

    unordered_map<char, string> encoding;
    encodingHuffman(nodeList, encoding);

    string compressedText = "";
    for (int i = 0; i < text.size(); i++) {
        compressedText += encoding[text[i]];
    }

    int zeroCount = 7 - compressedText.size() % 7;
    saveEncodingHuffman(encoding, zeroCount, encodingFilePath);

    while (zeroCount--) compressedText = "0" + compressedText;

    for (int i = 0; i < compressedText.size(); i += 7) {
        string part_string = compressedText.substr(i, 7);
        cText += char(stoi(part_string, 0, 2));
    }
}

void decompressTextHUFFMAN(string& text, string& dText, string& decodingFilePath) {
    unordered_map<string, char> decoding;
    int zero_count;
    readDecodingHuffman(decodingFilePath, decoding, zero_count);

    HuffmanNode* rootNode = makeTreeHuffman(decoding);

    string binaryText = "";
    for (auto element : text)
        binaryText += intToString7(element);

    binaryText = binaryText.substr(zero_count, binaryText.size() - zero_count);

    convertBinaryTextHuffman(binaryText, dText, rootNode);
}