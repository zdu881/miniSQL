// query.cpp
#include "query.hpp"
#include "parser.hpp"
#include "globals.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
Query::Query() {}

Query::~Query() {}

void Query::getQ() {
    std::cout << "MiniSQL> ";
    if (inputFile.is_open()) {
        std::string line;
        std::ostringstream queryStream;
        while (std::getline(inputFile, line)) {
            queryStream << line << " ";
            if (line.find(';') != std::string::npos) {
                break;
            }
        }
        userInput = queryStream.str();
    } else {
        std::getline(std::cin, userInput);
    }
}

void Query::excQ() {
    Parser parser;
    parser.parse(userInput, *dbptr);
}
/*
void Query::loaddb(Database& db){
    this->db = db;
}
*/