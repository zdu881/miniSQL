// table.hpp
#ifndef TABLE_HPP
#define TABLE_HPP

#include <string>
#include <vector>
#include <unordered_map>

class Table {
public:
    Table(const std::string& name);
    void insertRow(const std::unordered_map<std::string, std::string>& row);
    void deleteRow(int id);
    void queryTable() const;
private:
    std::string name;
    std::vector<std::unordered_map<std::string, std::string>> rows;
};

#endif // TABLE_HPP