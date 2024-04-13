#pragma once
#include <string>
ref class ReportGenerator
{
public:
    void generateReport();
    void writeCSVFile(const std::string& filename);
};

