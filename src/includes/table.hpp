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
    //void insertRow(const std::unordered_map<ColumnType, ColumnType>& row);
    void insertRow(const std::vector<ColumnType>& values);
    //void deleteRow(int id);
    void deleteRow(const std::vector<Condition>& conditions);
    void queryTable(const std::vector<std::string>& columns) const;
    void queryTable(const std::vector<std::string>& columns,const std::vector<Condition> &Conditions) const;
    void queryTable() const;
    void updateRow(const std::vector<std::pair<std::string, ColumnType>>& setConfigs, const std::vector<Condition>& conditions);
    void addColumn(const std::string& name, const Data_type& type);
    void save(std::ofstream& file) const;
    void load(std::ifstream& file);
    Data_type getColumnType(const std::string& columnName) const;
    friend std::ostream& operator<<(std::ostream& os, const Table& table);
    std::vector<std::pair<std::string, Data_type>> columnsNT;//Name-Type
    std::string name;
private:
    std::unordered_map<std::string, std::vector<ColumnType>> columns; // 列存储
};
#endif // TABLE_HPP