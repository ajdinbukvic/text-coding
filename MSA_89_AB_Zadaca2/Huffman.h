#pragma once
#include <vector>
#include "Symbol.h"
#include "SymbolExtended.h"
#include "Method.h"
class Huffman : public Method
{
public:
	void generateCodes(std::vector<Symbol>& symbols) override;
	void generateCodes(std::vector<SymbolExtended>& symbols) override;
};

