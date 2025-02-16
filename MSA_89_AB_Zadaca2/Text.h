#pragma once
#include <string>
#include <vector>
#include "MethodResult.h"
#include "SymbolExtended.h"

bool isAlnumUnicode(char32_t c);

bool isAllowedCharacter(char32_t c);

std::string symbolToString(char32_t c);

std::string symbolToString(const std::u32string& str);

std::u32string cleanText(const std::u32string& text);

std::u32string readAndCleanText(const std::string& filename);

void writeDataToCSV(const std::string& filename, const std::vector<Symbol>& symbols, const std::vector<SymbolExtended>& extendedSymbols, double entropy,
    int totalSymbols, size_t fileSize, MethodResult* mr);

void printUniqueUnicodeCharacters(const std::string& fileName);

void verifyDecoding(const std::string& originalFile, MethodResult* mr);

void convertCSVToXLSX(const std::string& csvFilename, const std::string& xlsxFilename);