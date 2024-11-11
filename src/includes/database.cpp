// database.cpp
#include "database.hpp"
#include <iostream>
#include <fstream>
#include "globals.hpp"
#include "table.hpp"
#include <algorithm>
void Database::createTable(const std::string& name, Table& table) {
    if (tables.find(name) == tables.end()) {
        tables[name] = std::move(table);
        std::cout << "Table " << name << " created." << std::endl;
    } else {
        std::cout << "Table " << name << " already exists." << std::endl;
    }
}

void Database::dropTable(const std::string& name) {
    if (tables.erase(name)) {
        std::cout << "Table " << name << " dropped." << std::endl;
    } else {
        std::cout << "Table " << name << " does not exist." << std::endl;
    }
}

Table* Database::getTable(const std::string& name) {
    auto it = tables.find(name);
    if (it != tables.end()) {
        return &it->second;
    }
    return nullptr;
}
void Database::innerJoinQuery(const std::vector<std::string> columns, const std::string& table1, const std::string& table2, const std::string& column1, const std::string& column2) {
    outputFile << "---------------------------------" << std::endl;
    for (size_t j = 0; j < columns.size(); ++j) {
        outputFile << columns[j];
        if (j < columns.size() - 1) {
            outputFile << ",";
        }
    }
    outputFile << std::endl;

    Table* t1 = getTable(table1);
    Table* t2 = getTable(table2);
    if (t1 == nullptr || t2 == nullptr) {
        std::cout << "Table not found" << std::endl;
        return;
    }

    // 获取连接列的索引
    auto col1Iter = std::find_if(t1->columnsNT.begin(), t1->columnsNT.end(), [&column1](const auto& pair) {
        return pair.first == column1;
    });
    auto col1Index = (col1Iter != t1->columnsNT.end()) ? std::distance(t1->columnsNT.begin(), col1Iter) : t1->columnsNT.size();
    auto col2Iter = std::find_if(t2->columnsNT.begin(), t2->columnsNT.end(), [&column2](const auto& pair) {
        return pair.first == column2;
    });
    auto col2Index = (col2Iter != t2->columnsNT.end()) ? std::distance(t2->columnsNT.begin(), col2Iter) : t2->columnsNT.size();

    if (col1Index == t1->columnsNT.size() || col2Index == t2->columnsNT.size()) {
        std::cout << "Column not found" << std::endl;
        return;
    }

    // 执行内连接查询
    for (size_t i = 0; i < t1->columns.at(column1).size(); ++i) {
        for (size_t j = 0; j < t2->columns.at(column2).size(); ++j) {
            if (t1->columns.at(column1)[i] == t2->columns.at(column2)[j]) {
                for (const auto& col : columns) {
                    if (t1->columns.find(col) != t1->columns.end()) {
                        outputFile << t1->columns.at(col)[i];
                    } else if (t2->columns.find(col) != t2->columns.end()) {
                        outputFile << t2->columns.at(col)[j];
                    }
                    outputFile << ",";
                }
                outputFile.seekp(-1, std::ios_base::end); // 移除最后一个逗号
                outputFile << std::endl;
            }
        }
    }
}
void Database::save(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return;
    }
    size_t tableCount = tables.size();
    file.write((char*)&tableCount, sizeof(tableCount));
    for (const auto& [name, table] : tables) {
        size_t nameLength = name.size();
        file.write((char*)&nameLength, sizeof(nameLength));
        file.write(name.c_str(), nameLength);
        table.save(file);
    }
    file.close();
}

void Database::load(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return;
    }
    size_t tableCount;
    file.read((char*)&tableCount, sizeof(tableCount));
    tables.clear();
    for (size_t i = 0; i < tableCount; ++i) {
        size_t nameLength;
        file.read((char*)&nameLength, sizeof(nameLength));
        std::string name(nameLength, '\0');
        file.read(&name[0], nameLength);
        Table table(name);
        table.load(file);
        tables[name] = table;
    }
}