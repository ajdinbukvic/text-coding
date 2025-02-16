#pragma once
#include "Method.h"
class ShannonFano : public Method
{
public:
	void generateCodes(std::vector<Symbol>& symbols, int start, int end, const std::string& prefix) override;
};

