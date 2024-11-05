// database.hpp
#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include <unordered_map>
#include "table.hpp"

class Database {
public:
    void createTable(const std::string& name);
    void dropTable(const std::string& name);
    Table* getTable(const std::string& name);
private:
    std::unordered_map<std::string, Table> tables;
};

#endif // DATABASE_HPP