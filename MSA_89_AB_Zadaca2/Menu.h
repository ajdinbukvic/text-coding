#pragma once
#include <string>
#include <vector>
#include "MethodResult.h"
#include "Method.h"

struct Data {
    double entropy;
    int totalSymbols;
    int totalUniqueSymbols;
    int fileSize;
    std::u32string text;
};

void readFileAndCalculateData(const std::string& inputFilename, std::vector<Symbol>& symbols, Data& data);

void generateCodesAndEncodeText(const std::string& inputFilename, std::vector<Method*> methods, MethodResult* mr, std::vector<Symbol>& symbols, std::vector<Symbol>& symbolVec, std::vector<SymbolExtended>& filteredSymbols,
    int blockSize, std::u32string text);

void decodeText(const std::string& outputFilename, std::vector<Method*> methods, MethodResult* mr);