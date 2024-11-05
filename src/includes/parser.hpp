// parser.hpp
#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include "database.hpp"

class Parser {
public:
    void parse(const std::string& input, Database& db);
};

#endif // PARSER_HPP