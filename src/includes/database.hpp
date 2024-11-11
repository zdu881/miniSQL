// database.hpp
#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include <unordered_map>
#include "table.hpp"
#include "globals.hpp"
class Database {
public:
    void createTable(const std::string& name, Table& table);
    void dropTable(const std::string& name);
    Table* getTable(const std::string& name);
    //SELECT column_name1, column_name2 FROM table_name1 INNER JOIN table_name2 ON table_name1.column_name1 = table_name2.column_name2
    void innerJoinQuery(const std::vector<std::string>columns, const std::string& table1, const std::string& table2 , const std::string& column1, const std::string& column2);
    void save(const std::string& filename);
    void load(const std::string& filename);
    std::unordered_map<std::string, Table> tables;
};

#endif // DATABASE_HPP