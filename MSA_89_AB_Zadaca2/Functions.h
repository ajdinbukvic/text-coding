#pragma once
#include <string>
#include <vector>
#include "MethodResult.h"
#include "SymbolExtended.h"
#include <map>

std::map<char32_t, int> calculateFrequencies(const std::u32string& text);

std::pair<std::vector<Symbol>, double> calculateRelativeFrequenciesAndEntropy(const std::map<char32_t, int>& frequencies, int totalSymbols);

bool checkKraftInequality(const std::vector<Symbol>& symbols, int index);

double calculateAverageCodeLength(const std::vector<Symbol>& symbols, int index);

double calculateEfficiency(const std::vector<Symbol>& symbols, int index);

double calculateRedundancy(double efficiency);

void displayResults(MethodResult* mr, const std::vector<Symbol>& symbols, const std::vector<Symbol>& symbolsCopy);

std::vector<Symbol> copyFromSymbolExtendedToSymbol(const std::vector<SymbolExtended>& extendedVec, int index);