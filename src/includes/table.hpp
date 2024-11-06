// table.hpp
#ifndef TABLE_HPP
#define TABLE_HPP

#include <string>
#include <vector>
#include <unordered_map>

class Table {
public:
    Table();
    Table(const std::string& name);
    void insertRow(const std::unordered_map<std::string, std::string>& row);
    void deleteRow(int id);
    void queryTable() const;
    void save(std::ofstream& file) const;
    friend std::ostream& operator<<(std::ostream& os, const Table& table);
private:
    std::string name;
    std::vector<std::unordered_map<std::string, std::string>> rows;
};

// Overload the << operator to print the table contents
#endif // TABLE_HPP