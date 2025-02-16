#pragma once
#include <vector>
#include <string>
#include "Symbol.h"
#include "SymbolExtended.h"
class Method
{
public:
	virtual void generateCodes(std::vector<Symbol>& symbols);
	virtual void generateCodes(std::vector<Symbol>& symbols, int start, int end, const std::string& prefix);
	virtual void generateCodes(std::vector<SymbolExtended>& symbols);
	virtual std::vector<SymbolExtended> generateCodes(std::vector<Symbol>& symbols, int blockFactor);
	virtual std::vector<SymbolExtended> filterCombinationsByText(const std::vector<SymbolExtended>& generatedSymbols, const std::u32string& text, int blockFactor);
	void encodeText(const std::string& inputFile, const std::vector<Symbol>& symbols, const std::string& outputFile, int index);
	void encodeText(const std::string& inputFile, const std::vector<SymbolExtended>& symbols, const std::string& outputFile, int blockSize, int index);
	void decodeTextFromCSV(const std::string& inputCSV, const std::string& encodedFile, const std::string& outputFile, int index);
	char32_t getSpecialCharacter(const std::u32string& word);
	std::u32string getSpecialCharacterFromStr(const std::u32string& word);
	std::u32string convertToU32String(const std::string& utf8str);
	void saveBinaryToFile(const std::string& binaryString, const std::string& filename);
	void readBinaryFromFile(const std::string& filename);
	Method() {}
};

