// parser.cpp
#include "parser.hpp"
#include <sstream>
#include <iostream>

void Parser::parse(const std::string& input, Database& db) {
    std::istringstream stream(input);
    std::string command;
    stream >> command;

    if (command == "CREATE") {
        std::string table;
        stream >> table;
        if (table == "TABLE") {
            std::string tableName;
            stream >> tableName;
            db.createTable(tableName);
        }
    } else if (command == "DROP") {
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
    }else {
        std::cout << "Unknown command: " << command << std::endl;
    }
}