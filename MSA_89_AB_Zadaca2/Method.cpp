#include "Method.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <codecvt>
#include "Symbol.h"
#include <fstream>
#include <map>
#include <string>
#include <bitset>

void Method::generateCodes(std::vector<Symbol>& symbols)
{
}

void Method::generateCodes(std::vector<SymbolExtended>& symbols)
{
}

void Method::generateCodes(std::vector<Symbol>& symbols, int start, int end, const std::string& prefix)
{
}

std::vector<SymbolExtended> Method::generateCodes(std::vector<Symbol>& symbols, int blockFactor)
{
    return std::vector<SymbolExtended>();
}

std::vector<SymbolExtended> Method::filterCombinationsByText(const std::vector<SymbolExtended>& generatedSymbols, const std::u32string& text, int blockFactor)
{
    return std::vector<SymbolExtended>();
}

void Method::encodeText(const std::string& inputFile, const std::vector<Symbol>& symbols, const std::string& outputFile, int index)
{
    std::map<char32_t, std::string> symbolToCodeMap;
    for (const auto& symbol : symbols) {
        symbolToCodeMap[symbol.character] = symbol.code[index];
    }

    std::ifstream inFile(inputFile);
    if (!inFile.is_open()) {
        std::cerr << "Greska kod otvaranja ulaznog fajla: " << inputFile << std::endl;
        exit(1);
    }

    std::string inputText((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;

    std::u32string utf32Text = converter.from_bytes(inputText);

    std::string encodedText = "";
    for (char32_t ch : utf32Text) {
        encodedText += symbolToCodeMap[ch];
    }

    std::ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        std::cerr << "Greska kod kreiranja fajla: " << outputFile << std::endl;
        exit(1);
    }

    outFile << encodedText;
    outFile.close();
    std::string outputFileBin = outputFile.substr(0, outputFile.length() - 4);
    outputFileBin += "_bin.txt";
    saveBinaryToFile(encodedText, outputFileBin);
    std::cout << "Kodiranje zavrseno. Kodirani tekst spremljen u: " << outputFile << std::endl;
}

void Method::encodeText(const std::string& inputFile, const std::vector<SymbolExtended>& symbols, const std::string& outputFile, int blockSize, int index)
{
    std::map<std::u32string, std::string> blockToCodeMap;
    for (const auto& symbol : symbols) {
        blockToCodeMap[symbol.combinedCharacters] = symbol.code[index];
    }

    std::ifstream inFile(inputFile);
    if (!inFile.is_open()) {
        std::cerr << "Greska kod otvaranja ulaznog fajla: " << inputFile << std::endl;
        exit(1);
    }

    std::string inputText((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::u32string utf32Text = converter.from_bytes(inputText);

    std::string encodedText = "";

    for (int i = 0; i < utf32Text.size(); i += blockSize) {
        std::u32string block;

        for (int j = i; j < i + blockSize && j < utf32Text.size(); j++) {
            block += utf32Text[j];
        }

        if (blockToCodeMap.find(block) != blockToCodeMap.end()) {
            encodedText += blockToCodeMap[block];
        }
    }

    std::ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        std::cerr << "Greska kod kreiranja fajla: " << outputFile << std::endl;
        exit(1);
    }

    outFile << encodedText;
    outFile.close();
    std::string outputFileBin = outputFile.substr(0, outputFile.length() - 4);
    outputFileBin += "_bin.txt";
    saveBinaryToFile(encodedText, outputFileBin);
    std::cout << "Kodiranje zavrseno. Kodirani tekst spremljen u: " << outputFile << std::endl;
}

void Method::decodeTextFromCSV(const std::string& inputCSV, const std::string& encodedFile, const std::string& outputFile, int index)
{
    std::ifstream file(inputCSV);
    if (!file.is_open()) {
        std::cerr << "Greska kod otvaranja CSV fajla: " << inputCSV << std::endl;
        exit(1);
    }

    std::map<std::string, char32_t> codeToSymbolMap;
    std::map<std::string, std::u32string> codeToSymbolMapStr;
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        if (line.empty()) {
            break;
        }
        std::stringstream ss(line);
        std::string symbolStr, frequency, relFreq, code0, code1, code2, code3;
        std::getline(ss, symbolStr, ',');
        std::getline(ss, frequency, ',');
        std::getline(ss, relFreq, ',');
        std::getline(ss, code0, ',');
        std::getline(ss, code1, ',');
        std::getline(ss, code2, ',');
        std::getline(ss, code3, ',');
        std::u32string u32Word = convertToU32String(symbolStr);
        char32_t symbol = getSpecialCharacter(u32Word);
        if (symbol == U'\0') {
            symbol = symbolStr[0];
        }
        if (index == 0) codeToSymbolMap[code0] = symbol;
        if (index == 1) codeToSymbolMap[code1] = symbol;
        if (index == 2) codeToSymbolMap[code2] = symbol;
        if (index == 3) codeToSymbolMap[code3] = symbol;
    }

    if (index == 4) {
        std::getline(file, line);
        while (std::getline(file, line)) {
            if (line.empty()) {
                break;
            }

            std::stringstream ss(line);
            std::string symbolStr, relFreq, multipliedHuffmanCode;
            std::getline(ss, symbolStr, ',');
            std::getline(ss, relFreq, ',');
            std::getline(ss, multipliedHuffmanCode, ',');

            std::u32string u32Word = convertToU32String(symbolStr);
            std::u32string symbol = getSpecialCharacterFromStr(u32Word);
            codeToSymbolMapStr[multipliedHuffmanCode] = symbol;
        }
    }

    file.close();

    std::ifstream encodedTextFile(encodedFile);
    if (!encodedTextFile.is_open()) {
        std::cerr << "Greska kod otvaranja kodiranog fajla." << std::endl;
        exit(1);
    }

    std::string encodedText;
    std::getline(encodedTextFile, encodedText);
    encodedTextFile.close();

    std::string currentCode = "";
    std::wstring decodedText = L"";

    if (index == 4) {
        for (char bit : encodedText) {
            currentCode += bit;

            if (codeToSymbolMapStr.find(currentCode) != codeToSymbolMapStr.end()) {
                std::u32string symbolGroup = codeToSymbolMapStr.at(currentCode);
                for (char32_t symbol : symbolGroup) {
                    decodedText += symbol;
                }
                currentCode.clear();
            }
        }
    }
    else {
        for (char bit : encodedText) {
            currentCode += bit;
            if (codeToSymbolMap.find(currentCode) != codeToSymbolMap.end()) {
                decodedText += codeToSymbolMap.at(currentCode);
                currentCode.clear();
            }
        }
    }

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string utf8Text = converter.to_bytes(decodedText);
    std::ofstream outputFileStream(outputFile);
    if (!outputFileStream.is_open()) {
        std::cerr << "Greska kod otvaranja dekodiranog fajla." << std::endl;
        exit(1);
    }

    outputFileStream << utf8Text;
    outputFileStream.close();

    std::cout << "Dekodiranje zavrseno. Dekodirani tekst spremljen u: " << outputFile << std::endl;
}

char32_t Method::getSpecialCharacter(const std::u32string& word)
{
    if (word == U"razmak") return U' ';
    if (word == U"zarez") return U',';
    if (word == U"novi_red") return U'\n';
    if (word == U"è") return U'è';
    if (word == U"æ") return U'æ';
    if (word == U"š") return U'š';
    if (word == U"ð") return U'ð';
    if (word == U"ž") return U'ž';
    if (word == U"È") return U'È';
    if (word == U"Æ") return U'Æ';
    if (word == U"Š") return U'Š';
    if (word == U"Ð") return U'Ð';
    if (word == U"Ž") return U'Ž';
    return U'\0';
}

std::u32string Method::getSpecialCharacterFromStr(const std::u32string& word)
{
    std::u32string result = U"";  
    int pos = 0;

    while (pos < word.length()) {
        if (word.compare(pos, 6, U"razmak") == 0) {  
            result += U' '; 
            pos += 6;  
        }
        else if (word.compare(pos, 5, U"zarez") == 0) { 
            result += U","; 
            pos += 5; 
        }
        else if (word.compare(pos, 8, U"novi_red") == 0) {  
            result += U'\n';  
            pos += 8;  
        }
        else if (word.compare(pos, 1, U"è") == 0) {  
            result += U"è";  
            pos += 1; 
        }
        else if (word.compare(pos, 1, U"æ") == 0) {  
            result += U"æ"; 
            pos += 1; 
        }
        else if (word.compare(pos, 1, U"š") == 0) { 
            result += U"š";  
            pos += 1;  
        }
        else if (word.compare(pos, 1, U"ð") == 0) {  
            result += U"ð";  
            pos += 1;  
        }
        else if (word.compare(pos, 1, U"ž") == 0) {  
            result += U"ž"; 
            pos += 1;  
        }
        else if (word.compare(pos, 1, U"È") == 0) {  
            result += U"È";  
            pos += 1;  
        }
        else if (word.compare(pos, 1, U"Æ") == 0) {  
            result += U"Æ"; 
            pos += 1; 
        }
        else if (word.compare(pos, 1, U"Š") == 0) {  
            result += U"Š";  
            pos += 1;  
        }
        else if (word.compare(pos, 1, U"Ð") == 0) {  
            result += U"Ð";  
            pos += 1;  
        }
        else if (word.compare(pos, 1, U"Ž") == 0) {  
            result += U"Ž";  
            pos += 1; 
        }
        else {
            result += word[pos];
            pos++;  
        }
    }

    return result;  
}

std::u32string Method::convertToU32String(const std::string& utf8str)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    return converter.from_bytes(utf8str);
}

void Method::saveBinaryToFile(const std::string& binaryString, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Greška kod kreiranja fajla!" << std::endl;
        return;
    }

    uint16_t bitLength = static_cast<uint16_t>(binaryString.size());  
    file.write(reinterpret_cast<const char*>(&bitLength), sizeof(bitLength));  

    std::vector<unsigned char> byteArray;
    for (size_t i = 0; i < binaryString.size(); i += 8) {
        std::string byteChunk = binaryString.substr(i, 8);
        while (byteChunk.length() < 8) {
            byteChunk += "0";
        }

        unsigned char byte = static_cast<unsigned char>(std::bitset<8>(byteChunk).to_ulong());
        byteArray.push_back(byte);
    }

    file.write(reinterpret_cast<const char*>(byteArray.data()), byteArray.size());
    file.close();
}

void Method::readBinaryFromFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Greška kod otvaranja fajla!" << std::endl;
        return;
    }

    uint16_t bitLength;
    file.read(reinterpret_cast<char*>(&bitLength), sizeof(bitLength)); 

    std::string binaryString;
    unsigned char byte;

    while (file.read(reinterpret_cast<char*>(&byte), 1)) {
        binaryString += std::bitset<8>(byte).to_string();  
    }

    binaryString = binaryString.substr(0, bitLength);

    file.close();
}