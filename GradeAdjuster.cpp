#include "GradeAdjuster.h"
#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>

void GradeAdjuster::displayTableByGrade() {
    // Connect to MySQL database
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;
    sql::Statement* stmt;
    sql::ResultSet* res;

    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "gumm");
        con->setSchema("gumm_schema");

        // Query to fetch data from Marks table in decreasing order of grades
        std::string query = "SELECT * FROM Marks2 ORDER BY Total DESC";
        stmt = con->createStatement();
        res = stmt->executeQuery(query);

        // Display the table
        while (res->next()) {
            std::cout << "StudentID: " << res->getInt("StudentID") << ", Name: " << res->getString("Name")
                << ", Total: " << res->getDouble("Total") << ", Grade: " << res->getString("Grade") << std::endl;
        }

        delete stmt;
        delete res;
        delete con;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Error: " << e.what() << std::endl;
    }
}

void GradeAdjuster::adjustStudentGrade(double targetMark, char targetGrade) {
    // Connect to MySQL database
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;
    sql::Statement* stmt;

    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "gumm");
        con->setSchema("gumm_schema");

        // Update grades of students based on target mark and grade
        // Note: here, "Grade >" is counterintuitive. This is because although we consider, for example, D to be a lower grade than C, its ASCII value is higher.
        std::string updateQuery = "UPDATE Marks2 SET Grade = '" + std::string(1, targetGrade) + "' WHERE Total >= " + std::to_string(targetMark) + " AND Grade > '" + std::string(1, targetGrade) + "'";

        stmt = con->createStatement();
        stmt->executeUpdate(updateQuery);

        delete stmt;
        delete con;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Error: " << e.what() << std::endl;
    }
}