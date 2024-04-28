#include "DataProcessor.h"
#include <iostream>
#include <vector>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>

void DataProcessor::calculateGrades() {
    // Connect to MySQL database
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;
    sql::Statement* stmt;
    sql::ResultSet* res;

    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "gumm");
        con->setSchema("gumm_schema");

        // Retrieve data from Marks table
        std::string query = "SELECT * FROM Marks2";
        stmt = con->createStatement();
        res = stmt->executeQuery(query);

        // Collect total marks for all students
        std::vector<double> totalMarks;
        while (res->next()) {
            double total = res->getDouble("Total");
            totalMarks.push_back(total);
        }

        // Calculate mean (average) of total marks
        double sum = 0.0;
        for (double mark : totalMarks) {
            sum += mark;
        }
        double mean = sum / totalMarks.size();

        // Calculate standard deviation
        double variance = 0.0;
        for (double mark : totalMarks) {
            variance += (mark - mean) * (mark - mean);
        }
        variance /= totalMarks.size();
        double stdDev = sqrt(variance);

        // Update grades based on standard deviation method
        res->beforeFirst();
        while (res->next()) {
            double mark = res->getDouble("Total");
            char grade;
            if (mark < mean / 2) {
                grade = 'F';
            }
            else if (mark >= mean / 2 && mark < mean - 2 * stdDev) {
                grade = 'E';
            }
            else if (mark >= mean - 2 * stdDev && mark < mean - stdDev) {
                grade = 'D';
            }
            else if (mark >= mean - stdDev && mark < mean) {
                grade = 'C';
            }
            else if (mark >= mean && mark < mean + stdDev) {
                grade = 'B';
            }
            else if (mark >= mean + stdDev && mark < mean + 2 * stdDev) {
                grade = 'A';
            }
            else {
                grade = 'S';
            }

            // Update Grade column in database
            int studentID = res->getInt("StudentID");
            std::string updateQuery = "UPDATE Marks2 SET Grade = '" + std::string(1, grade) + "' WHERE StudentID = " + std::to_string(studentID);
            stmt->executeUpdate(updateQuery);
        }

        delete stmt;
        delete res;
        delete con;
    }
    catch (sql::SQLException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}