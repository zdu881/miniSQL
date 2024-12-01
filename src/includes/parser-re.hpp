#ifndef PARSER_RE_HPP
#define PARSER_RE_HPP
#include <query.hpp>
#include <string>
#include <unordered_map>
#include <globals.hpp>
#include <vector>
#include <map>
class Parser{
    public :
        void parse(Command_line command_line, std::unordered_map<std::string, Database>* databases, std::string* currentDatabase);
    
};
ColumnType fromrawStringtoInt(ColumnType str);
ColumnType fromrawStringtoDouble(ColumnType str);
Data_type fromrawStringtoData_type(ColumnType str);
ColumnType evaluate(const std::string& expression, const std::map<std::string, ColumnType>& variables);
#endif // PARSER_RE_HPP