#include "HuffmanMultiplied.h"
#include <cmath>
#include <algorithm>
#include <queue>

std::set<std::u32string> HuffmanMultiplied::getTextBlockCombinations(const std::u32string& text, int blockFactor) {
    std::set<std::u32string> blockCombinations;
    auto len = text.length();

    for (int i = 0; i <= len - blockFactor; i++) {
        std::u32string block = text.substr(i, blockFactor);
        blockCombinations.insert(block);
    }

    return blockCombinations;
}

std::vector<SymbolExtended> HuffmanMultiplied::filterCombinationsByText(const std::vector<SymbolExtended>& generatedSymbols, const std::u32string& text, int blockFactor) {
    std::set<std::u32string> textCombinations = HuffmanMultiplied::getTextBlockCombinations(text, blockFactor);

    std::vector<SymbolExtended> filteredSymbols;

    for (const auto& symbol : generatedSymbols) {
        if (textCombinations.find(symbol.combinedCharacters) != textCombinations.end()) {
            filteredSymbols.push_back(symbol);
        }
    }

    return filteredSymbols;
}

std::vector<SymbolExtended> HuffmanMultiplied::generateCodes(std::vector<Symbol>& symbols, int blockFactor)
{
    std::vector<SymbolExtended> combinedSymbols;

    auto symbolCount = symbols.size();
    auto totalCombinations = std::pow(symbolCount, blockFactor); 

    for (int i = 0; i < symbolCount; i++) {
        for (int j = 0; j < symbolCount; j++) {
            if (blockFactor == 2) {
                std::u32string combinedChars = { symbols[i].character, symbols[j].character };
                double combinedRelativeFreq = symbols[i].relativeFrequency * symbols[j].relativeFrequency;
                int combinedFreq = symbols[i].frequency + symbols[j].frequency;
                SymbolExtended newBlockSymbol(combinedChars, combinedRelativeFreq);
                combinedSymbols.push_back(newBlockSymbol);
            }
            else if (blockFactor == 3) {
                for (int k = 0; k < symbolCount; k++) {
                    std::u32string combinedChars = { symbols[i].character, symbols[j].character, symbols[k].character };
                    double combinedRelativeFreq = symbols[i].relativeFrequency * symbols[j].relativeFrequency * symbols[k].relativeFrequency;
                    int combinedFreq = symbols[i].frequency + symbols[j].frequency + symbols[k].frequency;
                    SymbolExtended newBlockSymbol(combinedChars, combinedRelativeFreq);
                    combinedSymbols.push_back(newBlockSymbol);
                }
            }
        }
    }

    std::sort(combinedSymbols.begin(), combinedSymbols.end(), [](const SymbolExtended& a, const SymbolExtended& b) {
        return a.relativeFrequency > b.relativeFrequency;
    });

    return combinedSymbols;
}
