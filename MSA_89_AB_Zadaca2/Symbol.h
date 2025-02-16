#pragma once
#include <string>
class Symbol
{
public:
	char32_t character;
	int frequency;
	double relativeFrequency;
	std::string code[5];
	Symbol() : character(U'\0'), frequency(0), relativeFrequency(0.0) {
		for (int i = 0; i < 5; i++) {
			code[i] = "";
		}
	}
	Symbol(char32_t c, int freq, double relFreq) : character(c), frequency(freq), relativeFrequency(relFreq)
	{ 
		for (int i = 0; i < 5; i++) {
			code[i] = "";
		}
	}
};

