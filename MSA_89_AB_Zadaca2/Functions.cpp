#include <string>
#include <vector>
#include <cmath>
#include <map>
#include <iomanip>
#include <algorithm>
#include "Symbol.h"
#include "MethodResult.h"
#include <iostream>
#include "Functions.h"
#include "SymbolExtended.h"
#include <conio.h>

std::map<char32_t, int> calculateFrequencies(const std::u32string& text)
{
    std::map<char32_t, int> frequencies;
    for (char32_t c : text) {
        frequencies[c]++;
    }
    return frequencies;
}

std::pair<std::vector<Symbol>, double> calculateRelativeFrequenciesAndEntropy(const std::map<char32_t, int>& frequencies, int totalSymbols) {
    std::vector<Symbol> symbols;
    double entropy = 0.0;

    for (const auto& pair : frequencies) {
        char32_t character = pair.first;
        int freq = pair.second;

        double relativeFreq = static_cast<double>(freq) / totalSymbols;
        Symbol s = { character, freq, relativeFreq };
        symbols.push_back(s);
        entropy -= relativeFreq * std::log2(relativeFreq);
    }

    std::sort(symbols.begin(), symbols.end(), [](const Symbol& a, const Symbol& b) {
        return a.relativeFrequency > b.relativeFrequency;
    });

    return { symbols, entropy };
}

bool checkKraftInequality(const std::vector<Symbol>& symbols, int index) {
    double sum = 0.0;
    for (const auto& symbol : symbols) {
        sum += std::pow(2, -static_cast<int>(symbol.code[index].length()));
    }
    return sum <= 1.0;
}

double calculateAverageCodeLength(const std::vector<Symbol>& symbols, int index) {
    double avgLength = 0.0;
    for (const auto& symbol : symbols) {
        avgLength += symbol.relativeFrequency * symbol.code[index].length();
    }
    return avgLength;
}

double calculateEfficiency(const std::vector<Symbol>& symbols, int index) {
    double idealLength = 0.0;
    double avgLength = calculateAverageCodeLength(symbols, index);

    for (const auto& symbol : symbols) {
        double probability = symbol.relativeFrequency;
        idealLength -= probability * std::log2(probability);
    }

    return idealLength / avgLength;
}

double calculateRedundancy(double efficiency) {
    return 1.0 - efficiency;
}

void displayResults(MethodResult* mr, const std::vector<Symbol>& symbols, const std::vector<Symbol>& symbolsCopy)
{
    std::cout << "Ispis rezultata..." << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    std::cout << std::left << std::setw(25) << "Metoda"
        << std::setw(25) << "Kraftova nejednakost"
        << std::setw(30) << "Srednja duzina kodne rijeci"
        << std::setw(25) << "Efikasnost kodiranja"
        << std::setw(15) << "Redudancija"
        << std::endl;
    for (int i = 0; i < 5; i++) {
        if (i == 4) {
            if (checkKraftInequality(symbolsCopy, i)) {
                mr[i].kraftInequality = "zadovoljena";
            }
            else {
                mr[i].kraftInequality = "nije_zadovoljena";
            }
            mr[i].avgLength = calculateAverageCodeLength(symbolsCopy, i);
            mr[i].efficiency = calculateEfficiency(symbolsCopy, i);
            mr[i].redundancy = calculateRedundancy(mr[i].efficiency);
        }
        else {
            if (checkKraftInequality(symbols, i)) {
                mr[i].kraftInequality = "zadovoljena";
            }
            else {
                mr[i].kraftInequality = "nije_zadovoljena";
            }
            mr[i].avgLength = calculateAverageCodeLength(symbols, i);
            mr[i].efficiency = calculateEfficiency(symbols, i);
            mr[i].redundancy = calculateRedundancy(mr[i].efficiency);
        }
        std::cout << std::left << std::setw(25) << mr[i].name
            << std::setw(25) << mr[i].kraftInequality
            << std::setw(30) << mr[i].avgLength
            << std::setw(25) << mr[i].efficiency
            << std::setw(15) << mr[i].redundancy 
            << std::endl;
    }
    std::cout << "---------------------------------------------------" << std::endl;
    std::cout << "Pritisnite bilo koju tipku za nastavak..." << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    _getch();
}

std::vector<Symbol> copyFromSymbolExtendedToSymbol(const std::vector<SymbolExtended>& extendedVec, int index) {
    std::vector<Symbol> newVec;
    for (int i = 0; i < extendedVec.size(); i++) {
        Symbol s;
        s.relativeFrequency = extendedVec[i].relativeFrequency;
        s.code[index] = extendedVec[i].code[index];
        newVec.push_back(s);
    }
    return newVec;
}
