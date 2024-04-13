#include "ReportGenerator.h"
#include <iostream>
#include <fstream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>

void ReportGenerator::generateReport() {
    // Connect to MySQL database
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;
    sql::Statement* stmt;
    sql::ResultSet* res;

    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "gumm");
        con->setSchema("gumm_schema");

        // Generate report (pseudo code)
        std::string query = "SELECT Grade, COUNT(*) AS Count FROM Marks GROUP BY Grade";
        stmt = con->createStatement();
        res = stmt->executeQuery(query);

        while (res->next()) {
            std::cout << "Grade: " << res->getString("Grade") << ", Count: " << res->getInt("Count") << std::endl;
        }

        delete stmt;
        delete res;
        delete con;
    }
    catch (sql::SQLException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void ReportGenerator::writeCSVFile(const std::string& filename) {
    // Connect to MySQL database
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;
    sql::Statement* stmt;
    sql::ResultSet* res;

    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "gumm");
        con->setSchema("gumm_schema");

        // Fetch data from Marks table
        std::string query = "SELECT * FROM Marks";
        stmt = con->createStatement();
        res = stmt->executeQuery(query);

        // Open CSV file
        std::ofstream outfile("OutputFile.csv");
        if (!outfile.is_open()) {
            std::cerr << "Error: Could not open file. (reportGenerator)" << std::endl;
            return;
        }

        // Write data to CSV file
        while (res->next()) {
            outfile << res->getInt("StudentID") << ","
                << res->getString("Name") << ","
                << res->getDouble("Assessment1") << ","
                << res->getDouble("Assessment2") << ","
                << res->getDouble("Assignment") << ","
                << res->getDouble("Endsem") << ","
                << res->getDouble("Total") << ","
                << res->getString("Grade") << std::endl;
        }

        outfile.close();
        delete stmt;
        delete res;
        delete con;
    }
    catch (sql::SQLException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}