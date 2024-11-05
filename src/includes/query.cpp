// query.cpp
#include "query.hpp"
#include "parser.hpp"
#include <iostream>

Query::Query() {}

Query::~Query() {}

void Query::getQ() {
    std::cout << "MiniSQL> ";
    std::getline(std::cin, userInput);
}

void Query::excQ() {
    Parser parser;
    parser.parse(userInput, db);
}