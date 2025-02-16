#include "Huffman.h"
#include <algorithm>

void Huffman::generateCodes(std::vector<Symbol>& symbols)
{
    std::vector<std::pair<double, std::vector<Symbol*>>> workingSymbols;

    for (auto& symbol : symbols) {
        workingSymbols.emplace_back(symbol.relativeFrequency, std::vector<Symbol*>{&symbol});
    }

    while (workingSymbols.size() > 1) {
        auto right = workingSymbols.back(); workingSymbols.pop_back();
        auto left = workingSymbols.back(); workingSymbols.pop_back();

        for (auto* sym : left.second) {
            sym->code[2] = "0" + sym->code[2];
        }
        for (auto* sym : right.second) {
            sym->code[2] = "1" + sym->code[2];
        }

        double combinedFrequency = left.first + right.first;
        std::vector<Symbol*> combinedSymbols = left.second;
        combinedSymbols.insert(combinedSymbols.end(), right.second.begin(), right.second.end());

        workingSymbols.emplace_back(combinedFrequency, combinedSymbols);

        std::sort(workingSymbols.begin(), workingSymbols.end(), [](const auto& a, const auto& b) {
            return a.first > b.first;
        });
    }
}

void Huffman::generateCodes(std::vector<SymbolExtended>& symbols)
{
    std::vector<std::pair<double, std::vector<SymbolExtended*>>> workingSymbols;

    for (auto& symbol : symbols) {
        workingSymbols.emplace_back(symbol.relativeFrequency, std::vector<SymbolExtended*>{&symbol});
    }

    while (workingSymbols.size() > 1) {
        auto right = workingSymbols.back(); workingSymbols.pop_back();
        auto left = workingSymbols.back(); workingSymbols.pop_back();

        for (auto* sym : left.second) {
            sym->code[4] = "0" + sym->code[4];
        }
        for (auto* sym : right.second) {
            sym->code[4] = "1" + sym->code[4];
        }

        double combinedFrequency = left.first + right.first;
        std::vector<SymbolExtended*> combinedSymbols = left.second;
        combinedSymbols.insert(combinedSymbols.end(), right.second.begin(), right.second.end());

        workingSymbols.emplace_back(combinedFrequency, combinedSymbols);

        std::sort(workingSymbols.begin(), workingSymbols.end(), [](const auto& a, const auto& b) {
            return a.first > b.first;
        });
    }
}
