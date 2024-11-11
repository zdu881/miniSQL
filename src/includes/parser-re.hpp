#ifndef PARSER_RE_HPP
#define PARSER_RE_HPP
#include <query.hpp>
#include <string>
#include <unordered_map>
#include <globals.hpp>
#include <vector>

class Parser{
    public :
        void parse(Command_line command_line, std::unordered_map<std::string, Database>* databases, std::string* currentDatabase);
    
};
ColumnType fromrawStringtoInt(ColumnType str);
ColumnType fromrawStringtoDouble(ColumnType str);
Data_type fromrawStringtoData_type(ColumnType str);
#endif // PARSER_RE_HPP