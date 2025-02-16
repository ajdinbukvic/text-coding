#pragma once
#include <string>
class MethodResult
{
public:
	std::string name;
	std::string kraftInequality;
	double avgLength;
	double efficiency;
	double redundancy;
	double codingTime;
	double decodingTime;
	MethodResult() : name(""), kraftInequality(""), avgLength(0.0), efficiency(0.0), redundancy(0.0), codingTime(0.0), decodingTime(0.0) {}
};

