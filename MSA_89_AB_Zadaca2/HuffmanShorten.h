#pragma once
#include <vector>
#include "Symbol.h"
#include "Method.h"
class HuffmanShorten : public Method
{
public:
	void generateCodes(std::vector<Symbol>& symbols) override;
};

