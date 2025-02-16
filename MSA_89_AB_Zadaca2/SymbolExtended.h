#pragma once
#include <string>
#include "Symbol.h"
class SymbolExtended : public Symbol
{
public:
    std::u32string combinedCharacters;  
    SymbolExtended() : Symbol(), combinedCharacters(U"") {}   
    SymbolExtended(const std::u32string& combChars, double relFreq)
        : Symbol(), combinedCharacters(combChars) {
        this->relativeFrequency = relFreq;
        this->frequency = 0;  
        this->character = U'\0'; 
    }
};

