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
        // std::cout << "Table " << name << " created." << std::endl;
    } else {
         std::cerr << "Command " << linenumber<<": " << "Table " << name << " already exists." << std::endl;
    }
}

void Database::dropTable(const std::string& name) {
    if (tables.erase(name)) {
        // std::cout << "Table " << name << " dropped." << std::endl;
    } else {
        // std::cout << "Table " << name << " does not exist." << std::endl;
    }
}

Table* Database::getTable(const std::string& name) {
    auto it = tables.find(name);
    if (it != tables.end()) {
        return &it->second;
    }
    return nullptr;
}
void Database::innerJoinQuery(std::vector<std::string> columns, const std::string& table1, const std::string& table2, const std::string& column1, const std::string& column2) {
    
    for (size_t j = 0; j < columns.size(); ++j) {
        outputFile << columns[j];
        if (j < columns.size() - 1) {
            outputFile << ",";
        }
    }
    outputFile << std::endl;
    for(auto& i:columns) i = i.substr(i.find('.') + 1);
    Table* t1 = getTable(table1);
    Table* t2 = getTable(table2);
    if (t1 == nullptr || t2 == nullptr) {
         std::cerr << "Table not found" << std::endl;
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
        std::cerr << "Column not found" << std::endl;
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
    outputFile << "---------------------------------" << std::endl;
}
void Database::innerJoinQueryWHERE(std::vector<std::string> columns, const std::string& table1, const std::string& table2, const std::string& column1, const std::string& column2, const std::vector<Condition>& conditions) {

    for (size_t j = 0; j < columns.size(); ++j) {
        outputFile << columns[j];
        if (j < columns.size() - 1) {
            outputFile << ",";
        }
    }
    outputFile << std::endl;
    for(auto& i:columns) i = i.substr(i.find('.') + 1);
    Table* t1 = getTable(table1);
    Table* t2 = getTable(table2);
    if (t1 == nullptr || t2 == nullptr) {
        std::cerr << "Table not found" << std::endl;
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
        std::cerr << "Column not found" << std::endl;
        return;
    }
    // 执行内连接查询并应用条件
    for (size_t i = 0; i < t1->columns.at(column1).size(); ++i) {
        for (size_t j = 0; j < t2->columns.at(column2).size(); ++j) {
            if (t1->columns.at(column1)[i] == t2->columns.at(column2)[j]) {
                bool match = true;
                for (const auto& condition : conditions) {
                    const auto& colData = t1->columns.find(condition.column) != t1->columns.end() ? t1->columns.at(condition.column) : t2->columns.at(condition.column);
                    Data_type colType = t1->columns.find(condition.column) != t1->columns.end() ? t1->getColumnType(condition.column) : t2->getColumnType(condition.column);
                    if (colType == ERROR_TYPE) {
                        match = false;
                        break;
                    }
                    bool conditionMatch = false;
                    if (colType == INTEGER) {
                        int value = std::stoi(std::get<std::string>(condition.value));
                        int arg = std::get<int>(colData[i]);
                        switch (condition.sign) {
                            case EQUAL: conditionMatch = (arg == value); break;
                            case UNEQUAL: conditionMatch = (arg != value); break;
                            case SMALLER: conditionMatch = (arg < value); break;
                            case BIGGER: conditionMatch = (arg > value); break;
                            case LESS_EQUAL: conditionMatch = (arg <= value); break;
                            case GREATER_EQUAL: conditionMatch = (arg >= value); break;
                            default: conditionMatch = false; break;
                        }
                    } else if (colType == FLOAT) {
                        double value = std::stod(std::get<std::string>(condition.value));
                        double arg = std::get<double>(colData[i]);
                        switch (condition.sign) {
                            case EQUAL: conditionMatch = (arg == value); break;
                            case UNEQUAL: conditionMatch = (arg != value); break;
                            case SMALLER: conditionMatch = (arg < value); break;
                            case BIGGER: conditionMatch = (arg > value); break;
                            case LESS_EQUAL: conditionMatch = (arg <= value); break;
                            case GREATER_EQUAL: conditionMatch = (arg >= value); break;
                            default: conditionMatch = false; break;
                        }
                    } else if (colType == TEXT) {
                        const std::string& value = std::get<std::string>(condition.value);
                        const std::string& arg = std::get<std::string>(colData[i]);
                        switch (condition.sign) {
                            case EQUAL: conditionMatch = (arg == value); break;
                            case UNEQUAL: conditionMatch = (arg != value); break;
                            default: conditionMatch = false; break;
                        }
                    }
                    if (!conditionMatch) {
                        match = false;
                        break;
                    }
                }
                if (match) {
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
    outputFile << "---------------------------------" << std::endl;
}
void Database::save(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        // std::cerr << "Command " << linenumber<<": " << "Cannot open file: " << filename << std::endl;
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
        // std::cerr << "Command " << linenumber<<": " << "Cannot open file: " << filename << std::endl;
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