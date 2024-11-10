// table.cpp
#include "table.hpp"
#include <iostream>
#include <fstream>
#include <variant>
#include <algorithm>
#include <unordered_map>
#include <string>
#include "globals.hpp"
Table::Table() : name(""), columnsNT() {}
Table::Table(const Table& other) : name(other.name), columnsNT(other.columnsNT), columns(other.columns) {}
Table::Table(const std::string& name) : name(name), columnsNT() {}
// 添加辅助函数以从 columnsNT 获取数据类型
Data_type Table::getColumnType(const std::string& columnName) const {
    for (const auto& pair : columnsNT) {
        if (pair.first == columnName) {
            return pair.second;
        }
    }
    return ERROR_TYPE;
}
void Table::insertRow(const std::vector<ColumnType>& values) {
    if (values.size() != columnsNT.size()) {
        std::cerr << "Column count mismatch. Expected " << columnsNT.size() << " values." << std::endl;
        return;
    }
    for (size_t i = 0; i < columnsNT.size(); ++i) {
        columns[columnsNT[i].first].push_back(values[i]);
    }
    std::cout << "Row inserted into " << name << "." << std::endl;
}


void Table::queryTable() const {
    // std::cout 替换为 outputFile
    outputFile << "Table " << name << " contents:" << std::endl;
    for (size_t i = 0; i < columns.begin()->second.size(); ++i) {
        for (const auto& column : columnsNT) {
            const auto& colData = columns.at(column.first);
            std::visit([this](auto&& arg) {
                if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::string>) {
                    outputFile << "\"" << arg << "\" ";
                } else {
                    outputFile << arg << " ";
                }
            }, colData[i]);
        }
        outputFile << std::endl;
    }
}

void Table::queryTable(const std::vector<std::string>& columns) const {
    // std::cout 替换为 outputFile
    outputFile << "Table " << name << " contents:" << std::endl;
    size_t rowCount = this->columns.begin()->second.size();
    std::cout << "Table " << name << " contents with conditions:" << std::endl;

    for (size_t i = 0; i < rowCount; ++i) {
        for (const auto& column : columns) {
            if (this->columns.find(column) == this->columns.end()) {
                std::cerr << "Column " << column << " does not exist in table " << name << std::endl;
                continue;
            }

            const auto& colData = this->columns.at(column);

            if (this->getColumnType(column) == INTEGER || this->getColumnType(column) == FLOAT) {
                outputFile << " " << colData[i] << " " << std::endl;
            } else if (this->getColumnType(column) == TEXT) {
                outputFile << " \"" << std::get<std::string>(colData[i]) << "\" " << std::endl;
            } else {
                std::cerr << "ERROR: Unknown column type for column " << column << std::endl;
            }
        }
        outputFile << std::endl;
    }
    std::cout << "QueryTable" << std::endl;
}



void Table::queryTable(const std::vector<std::string>& columns, const std::vector<Condition>& conditions) const {
    // std::cout 替换为 outputFile
    outputFile << "Table " << name << " contents with conditions:" << std::endl;
    // 输出列名
    for (const auto& column : columns) {
        outputFile << column << " ";
    }
    outputFile << std::endl;

    // 输出符合条件的行
    for (size_t i = 0; i < this->columns.begin()->second.size(); ++i) {
        bool match = true;
        for (const auto& condition : conditions) {
            const auto& colData = this->columns.at(condition.column);
            // 使用辅助函数获取列的数据类型
            Data_type colType = getColumnType(condition.column);
            if (colType == ERROR_TYPE) {
                outputFile << "Column " << condition.column << " does not exist." << std::endl;
                match = false;
                break;
            }
            bool conditionMatch = std::visit([&condition, colType](ColumnType&& arg) -> bool {
                using T = std::decay_t<decltype(arg)>;
                if (colType == INTEGER) {
                    int condValue = std::stoi(std::get<std::string>(condition.value));
                    if (condition.sign == EQUAL) return std::get<int>(arg) == condValue;
                    else if (condition.sign == BIGGER) return std::get<int>(arg) > condValue;
                    else if (condition.sign == SMALLER) return std::get<int>(arg) < condValue;
                } else if (colType == FLOAT) {
                    double condValue = std::stod(std::get<std::string>(condition.value));
                    if (condition.sign == EQUAL) return std::get<double>(arg) == condValue;
                    else if (condition.sign == BIGGER) return std::get<double>(arg) > condValue;
                    else if (condition.sign == SMALLER) return std::get<double>(arg) < condValue;
                } else if (colType == TEXT) {
                    if (condition.sign == EQUAL) return std::get<std::string>(arg) == std::get<std::string>(condition.value);
                }
                return false;
            }, colData[i]);

            if (!conditionMatch) {
                match = false;
                break;
            }
        }

        if (match) {
            for (const auto& column : columns) {
                const auto& colData = this->columns.at(column);
                std::visit([](auto&& arg) {
                    if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::string>) {
                        outputFile << "\"" << arg << "\" ";
                    } else {
                        outputFile << arg << " ";
                    }
                }, colData[i]);
            }
            outputFile << std::endl;
        }
    }
}

void Table::deleteRow(const std::vector<Condition>& conditions) {
    std::vector<size_t> rowsToDelete;
    for (size_t i = 0; i < columns.begin()->second.size(); ++i) {
        bool deleteRow = true;
        for (const auto& condition : conditions) {
            const auto& colData = columns.at(condition.column);
            const auto& colType = std::find_if(columnsNT.begin(), columnsNT.end(), 
                                               [&condition](const auto& pair) { return pair.first == condition.column; })->second;
        //wait to write
        }
        if (deleteRow) {
            rowsToDelete.push_back(i);
        }
    }
    for (auto& [columnName, columnData] : columns) {
        for (size_t i = 0; i < rowsToDelete.size(); ++i) {
            columnData.erase(columnData.begin() + rowsToDelete[i]);
        }
    }
    std::cout << rowsToDelete.size() << " rows deleted from " << name << "." << std::endl;
}
void Table::updateRow(const std::vector<std::pair<std::string, ColumnType>>& setConfigs, const std::vector<Condition>& conditions){
    std::vector<size_t> rowsToUpdate;
    for (size_t i = 0; i < columns.begin()->second.size(); ++i) {
        bool updateRow = true;
        for (const auto& condition : conditions) {
            const auto& colData = columns.at(condition.column);
            const auto& colType = std::find_if(columnsNT.begin(), columnsNT.end(), 
                                               [&condition](const auto& pair) { return pair.first == condition.column; })->second;
            //wait to write
        }
        if (updateRow) {
            rowsToUpdate.push_back(i);
        }
    }
    for ( auto& [columnName, columnData] : columns) {
        for (size_t i = 0; i < rowsToUpdate.size(); ++i) {
            columnData[rowsToUpdate[i]] = std::find_if(setConfigs.begin(), setConfigs.end(), 
                                                       [&columnName](const auto& pair) { return pair.first == columnName; })->second;
        }
    }
    std::cout << rowsToUpdate.size() << " rows updated in " << name << "." << std::endl;
}
void Table::save(std::ofstream& file) const {

}

void Table::load(std::ifstream& file) {
    
}

void Table::addColumn(const std::string& name, const Data_type& type) {
    columnsNT.push_back({name, type});
    std::cout << "Column " << name << " of type " << type << " added to " << this->name << "." << std::endl;
}
