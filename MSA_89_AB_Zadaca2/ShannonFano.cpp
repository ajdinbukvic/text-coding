#include "ShannonFano.h"

void ShannonFano::generateCodes(std::vector<Symbol>& symbols, int start, int end, const std::string& prefix)
{
    if (start == end) {
        symbols[start].code[1] = prefix;
        return;
    }

    int totalFrequency = 0;
    for (int i = start; i <= end; i++) {
        totalFrequency += symbols[i].frequency;
    }

    int halfFrequency = totalFrequency / 2;
    int sum = 0;
    int splitIndex = start;

    for (int i = start; i <= end; i++) {
        sum += symbols[i].frequency;
        if (sum >= halfFrequency) {
            splitIndex = i;
            break;
        }
    }

    if (splitIndex >= end) {
        splitIndex = end - 1;
    }

    generateCodes(symbols, start, splitIndex, prefix + "0");
    generateCodes(symbols, splitIndex + 1, end, prefix + "1");
}