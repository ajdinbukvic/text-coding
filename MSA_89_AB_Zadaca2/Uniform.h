#pragma once
#include "Method.h"

class Uniform : public Method {
public:
	void generateCodes(std::vector<Symbol>& symbols) override;
};

