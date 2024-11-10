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
    void save(const std::string& filename);
    void load(const std::string& filename);
    std::unordered_map<std::string, Table> tables;
};

#endif // DATABASE_HPP