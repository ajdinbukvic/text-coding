#include "Uniform.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <codecvt>
#include <fstream>
#include <map>
#include <bitset>

void Uniform::generateCodes(std::vector<Symbol>& symbols)
{
    auto n = symbols.size();
    int codeLength = static_cast<int>(std::ceil(std::log2(n)));

    for (int i = 0; i < n; i++) {
        std::bitset<32> code(i);
        std::string codeStr = code.to_string().substr(32 - codeLength);
        symbols[i].code[0] = codeStr;
    }
}