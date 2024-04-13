#pragma once
#include "DataProcessor.h"
ref class SDMethod : public DataProcessor
{
private:
	int average, standardDeviation;
public:
	void calcAverage();
	void calcSD();
};

