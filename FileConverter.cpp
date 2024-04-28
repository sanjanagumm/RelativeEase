#include "FileConverter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>

void FileConverter::convertCSVtoDatabase(const std::string& filename) {
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;
    sql::Statement* stmt;
    sql::ResultSet* res = 0;

    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "gumm");
        con->setSchema("gumm_schema");

        std::ifstream file("InputFile2.csv");
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file. (fileConverter)" << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string token;
            std::vector<std::string> tokens;
            while (std::getline(iss, token, ',')) {
                tokens.push_back(token);
            }

            // Assuming table structure: (StudentID INT, Name VARCHAR(255), Assessment1 FLOAT, Assessment2 FLOAT, Assignment FLOAT, Total FLOAT)
            std::string query = "INSERT INTO Marks2 (StudentID, Name, Assessment1, Assessment2, Assignment, Endsem, Total) VALUES ('"
                + tokens[0] + "', '" + tokens[1] + "', '" + tokens[2] + "', '" + tokens[3] + "', '"
                + tokens[4] + "', '" + tokens[5] + "', '" + tokens[6] + "')";

            stmt = con->createStatement();
            stmt->execute(query);
            delete stmt;
        }

        file.close();
        delete con;
    }
    catch (sql::SQLException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}