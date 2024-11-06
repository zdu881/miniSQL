// parser.cpp
#include "parser.hpp"
#include <sstream>
#include <iostream>
#include <fstream>
void Parser::parse(const std::string& input, Database& db) {
    std::istringstream stream(input);
    std::string command;
    stream >> command;
    if(command == "USE"){
        std::string dbName;
        stream>>dbName;
        std::ifstream file(dbName+".db", std::ios::in);
        if (!file){
            std::cerr<<"Cannot open the database:"<<dbName<<std::endl;
            return;
        }
        db.load(dbName+".db");
        file.close();
    }
    else if (command == "CREATE") {
        std::string input;
        stream >> input;
        if(input=="DATABASE"){
            std::string dbName;
            stream>>dbName;
            std::ofstream file(dbName+".db", std::ios::out);
            if (!file){
                std::cerr<<"Cannot create the database:"<<dbName<<std::endl;
                return;
            }
            file.close();
        }
        else if (input == "TABLE") {
            //check if the database is loaded
            if (&db==nullptr){
                std::cout<<"Please load a database first."<<std::endl;
                return;
            }
            std::string tableName;
            stream >> tableName;
            db.createTable(tableName);
            // input CREATE TABLE table_name (column_name column_type, ...)

            stream >> input; // (

            std::string column;
            while (stream >> column) {
                if (column == ")") {
                    break;
                }
                std::string columnName = column;
                stream >> column; // column type
                std::string columnType = column;
                db.getTable(tableName)->addColumn(columnName, columnType);
            }
        }
     else if (command == "DROP") {
        std::string table;
        stream >> table;
        if (table == "TABLE") {
            std::string tableName;
            stream >> tableName;
            db.dropTable(tableName);
        }
    } else if (command == "SELECT") {
        std::string tableName;
        stream >> tableName;
        Table* table = db.getTable(tableName);
        if (table) {
            table->queryTable();
        } else {
            std::cout << "Table " << tableName << " does not exist." << std::endl;
        }
    }else if(command == "EXIT"){
        std::cout<<"See ya next time!"<<std::endl;
        exit(0);
    }else if(command == "HELP"){
        std::cout<<"Commands available: CREATE TABLE, DROP TABLE, SELECT, EXIT"<<std::endl;
    }
    else {
        std::cout << "Unknown command: " << command << std::endl;
    }
}