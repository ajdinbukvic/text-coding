#include "HuffmanShorten.h"
#include <cmath>

void HuffmanShorten::generateCodes(std::vector<Symbol>& symbols)
{
    double cumulativeFrequency = 0.0;

    for (auto& symbol : symbols) {
        double midPoint = cumulativeFrequency + symbol.relativeFrequency / 2.0;
        cumulativeFrequency += symbol.relativeFrequency;

        std::string binaryCode = "";
        double fraction = midPoint;
        int maxLength = static_cast<int>(std::ceil(-std::log2(symbol.relativeFrequency))); 

        for (int i = 0; i < maxLength; i++) {
            fraction *= 2;
            if (fraction >= 1.0) {
                binaryCode += "1";
                fraction -= 1.0;
            }
            else {
                binaryCode += "0";
            }
        }

        symbol.code[3] = binaryCode;
    }
}
