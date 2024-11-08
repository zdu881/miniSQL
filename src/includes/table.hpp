// table.hpp
#ifndef TABLE_HPP
#define TABLE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include "globals.hpp"
#include <fstream>
class Table {
public:
    Table();
    Table(const Table& other);
    Table(const std::string& name);
    void insertRow(const std::unordered_map<ColumnType, ColumnType>& row);
    void insertRow(const std::vector<ColumnType>& values);
    void deleteRow(int id);
    void deleteRows(const std::string& whereColumn, const std::string& whereOperator, const ColumnType& whereValue);
    void queryTable(const std::vector<std::string>& columns) const;
    void queryTable(const std::vector<std::string>& columns, const std::string& whereColumn, const std::string& whereOperator, const ColumnType& whereValue) const;
    void queryTable() const;
    void addColumn(const std::string& name, const std::string& type);
    void updateRow(const std::string& columnName, const std::string& operation, const ColumnType& value);
    void updateColumn(const std::string& columnName, const std::string& operation, const ColumnType& value);
    void updateColumn(const std::string& columnName, const std::string& operation, const ColumnType& value, const std::string& whereColumn, const std::string& whereOperator, const ColumnType& whereValue);
    void save(std::ofstream& file) const;
    void load(std::ifstream& file);
    friend std::ostream& operator<<(std::ostream& os, const Table& table);
private:
    std::vector<std::pair<std::string, std::string>> columnsNT; // Name - Type
    std::unordered_map<std::string, std::vector<ColumnType>> columns; // 列存储
    std::string name;
};

#endif // TABLE_HPP