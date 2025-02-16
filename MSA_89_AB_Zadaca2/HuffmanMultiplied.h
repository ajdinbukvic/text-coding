#pragma once
#include <vector>
#include "Symbol.h"
#include "SymbolExtended.h"
#include "Method.h"
#include <set>
class HuffmanMultiplied : public Method
{
public:
	static std::set<std::u32string> getTextBlockCombinations(const std::u32string& text, int blockFactor);
	std::vector<SymbolExtended> filterCombinationsByText(const std::vector<SymbolExtended>& generatedSymbols, const std::u32string& text, int blockFactor) override;
	std::vector<SymbolExtended> generateCodes(std::vector<Symbol>& symbols, int blockFactor) override;
};

