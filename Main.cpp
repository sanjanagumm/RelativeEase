#include <iostream>
#include <fstream>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include "FileConverter.h"
#include "DataProcessor.h"
#include "GradeAdjuster.h"
#include "ReportGenerator.h"
#include <mysql_driver.h>

// Function to execute SQL script
void executeScript(sql::Connection* con, const std::string& filename) {
    std::ifstream file("SQLScript.sql");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open SQL script file." << std::endl;
        return;
    }

    std::string line, script;
    while (std::getline(file, line)) {
        script += line;
    }
    file.close();

    try {
        sql::Statement* stmt = con->createStatement();
        stmt->execute(script);
        delete stmt;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Error: " << e.what() << std::endl;
    }
}

int main() {
    // Connect to MySQL database
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;

    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "gumm");
        con->setSchema("gumm_schema");

        // Execute SQL script to create table
        executeScript(con, "SQLScript.sql");

        // Use FileConverter to convert InputFile.csv to database
        FileConverter converter;
        converter.convertCSVtoDatabase("InputFile2.csv");

        // Use DataProcessor to perform relative grading
        DataProcessor processor;
        processor.calculateGrades();

        // Use GradeAdjuster to display table by grade and adjust grades
        GradeAdjuster adjuster;
        adjuster.displayTableByGrade();

        // Prompt user to adjust grades
        char choice;
        do {
            double targetMark;
            char targetGrade;
            std::cout << "Do you want to adjust grades? (y/n): ";
            std::cin >> choice;
            if (choice == 'y') {
                std::cout << "Enter target mark: ";
                std::cin >> targetMark;
                std::cout << "Enter target grade: ";
                std::cin >> targetGrade;
                adjuster.adjustStudentGrade(targetMark, targetGrade);
                adjuster.displayTableByGrade();
            }
        } while (choice == 'y');

        // Use ReportGenerator to generate report and write to OutputFile.csv
        ReportGenerator reportGenerator;
        reportGenerator.generateReport();
        reportGenerator.writeCSVFile("OutputFile2.csv");


        delete con;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Error: " << e.what() << std::endl;
    }

    return 0;
}
