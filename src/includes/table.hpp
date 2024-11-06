// table.hpp
#ifndef TABLE_HPP
#define TABLE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include "globals.hpp"
class Table {
public:
    Table();
    Table(const std::string& name);
    void insertRow(const std::unordered_map<ColumnType, ColumnType>& row);
    void deleteRow(int id);
    void queryTable() const;
    void addColumn(const std::string& name, const std::string& type);
    void save(std::ofstream& file) const;
    void load(std::ifstream& file);
    friend std::ostream& operator<<(std::ostream& os, const Table& table);
private:
    std::string name;
    std::vector<std::unordered_map<ColumnType,ColumnType>> rows;
};

// Overload the << operator to print the table contents
#endif // TABLE_HPP