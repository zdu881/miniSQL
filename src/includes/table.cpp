// table.cpp
#include "table.hpp"
#include <iostream>
#include <fstream>
#include <variant>
#include <algorithm>
#include <unordered_map>
#include "globals.hpp"
#include <iomanip>
Table::Table() : name(""), columnsNT() {}

Table::Table(const std::string& name) : name(name), columnsNT() {}

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

void Table::deleteRows(const std::string& whereColumn, const std::string& whereOperator, const ColumnType& whereValue) {
    const auto& whereColData = columns.at(whereColumn);
    for (size_t i = 0; i < whereColData.size(); ++i) {
        bool conditionMet = false;
        if (whereOperator == "=") {
            conditionMet = (whereColData[i] == whereValue);
        } else if (whereOperator == "<") {
            conditionMet = (whereColData[i] < whereValue);
        } else if (whereOperator == ">") {
            conditionMet = (whereColData[i] > whereValue);
        }
        if (conditionMet) {
            for (auto& [colName, colData] : columns) {
                colData.erase(colData.begin() + i);
            }
            --i; // Adjust index after deletion
        }
    }
    std::cout << "Rows deleted from " << name << " where " << whereColumn << " " << whereOperator << " " << whereValue << "." << std::endl;
}

void Table::queryTable() const {
    std::cout << "Table " << name << " contents:" << std::endl;
    for (size_t i = 0; i < columns.begin()->second.size(); ++i) {
        for (const auto& column : columnsNT) {
            const auto& colData = columns.at(column.first);
            std::visit([](auto&& arg) {
                if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::string>) {
                    std::cout << "\"" << arg << "\" ";
                } else if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, double>) {
                    std::cout << std::fixed << std::setprecision(2) << arg << " ";
                } else {
                    std::cout << arg << " ";
                }
            }, colData[i]);
        }
        std::cout << std::endl;
    }
}

void Table::queryTable(const std::vector<std::string>& columns) const {
    std::cout << "Table " << name << " contents:" << std::endl;
    size_t rowCount = this->columns.begin()->second.size();
    for (size_t i = 0; i < rowCount; ++i) {
        for (const auto& column : columns) {
            const auto& colData = this->columns.at(column);
            std::visit([](auto&& arg) {
                if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::string>) {
                    std::cout << "\"" << arg << "\" ";
                } else if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, double>) {
                    std::cout << std::fixed << std::setprecision(2) << arg << " ";
                } else {
                    std::cout << arg << " ";
                }
            }, colData[i]);
        }
        std::cout << std::endl;
    }
}

void Table::queryTable(const std::vector<std::string>& columns, const std::string& whereColumn, const std::string& whereOperator, const ColumnType& whereValue) const {
    std::cout << "Table " << name << " contents:" << std::endl;
    const auto& whereColData = this->columns.at(whereColumn);
    size_t rowCount = whereColData.size();
    for (size_t i = 0; i < rowCount; ++i) {
        bool conditionMet = false;
        if (whereOperator == "=") {
            conditionMet = (whereColData[i] == whereValue);
        } else if (whereOperator == "<") {
            conditionMet = (whereColData[i] < whereValue);
        } else if (whereOperator == ">") {
            conditionMet = (whereColData[i] > whereValue);
        }
        if (conditionMet) {
            for (const auto& column : columns) {
                const auto& colData = this->columns.at(column);
                std::visit([](auto&& arg) {
                    if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::string>) {
                        std::cout << "\"" << arg << "\" ";
                    } else if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, double>) {
                        std::cout << std::fixed << std::setprecision(2) << arg << " ";
                    } else {
                        std::cout << arg << " ";
                    }
                }, colData[i]);
            }
            std::cout << std::endl;
        }
    }
}

void Table::save(std::ofstream& file) const {
    auto rowCount = columns.begin()->second.size();
    file.write((char*)&rowCount, sizeof(rowCount));
    
    auto columnCount = columnsNT.size();
    file.write((char*)&columnCount, sizeof(columnCount));
    for (const auto& column : columnsNT) {
        auto nameLength = column.first.size();
        file.write((char*)&nameLength, sizeof(nameLength));
        file.write(column.first.c_str(), nameLength);
        
        auto typeLength = column.second.size();
        file.write((char*)&typeLength, sizeof(typeLength));
        file.write(column.second.c_str(), typeLength);
    }

    for (const auto& [colName, colData] : columns) {
        for (const auto& value : colData) {
            std::visit([&file](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, int>) {
                    char type = 'i';
                    file.write(&type, sizeof(type));
                    file.write((char*)&arg, sizeof(arg));
                } else if constexpr (std::is_same_v<T, double>) {
                    char type = 'd';
                    file.write(&type, sizeof(type));
                    file.write((char*)&arg, sizeof(arg));
                } else if constexpr (std::is_same_v<T, std::string>) {
                    char type = 's';
                    file.write(&type, sizeof(type));
                    auto length = arg.size();
                    file.write((char*)&length, sizeof(length));
                    file.write(arg.c_str(), length);
                }
            }, value);
        }
    }
}

void Table::load(std::ifstream& file) {
    size_t rowCount;
    file.read((char*)&rowCount, sizeof(rowCount));
    
    size_t columnCount;
    file.read((char*)&columnCount, sizeof(columnCount));
    columnsNT.clear();
    columns.clear();
    for (size_t i = 0; i < columnCount; ++i) {
        size_t nameLength;
        file.read((char*)&nameLength, sizeof(nameLength));
        std::string name(nameLength, '\0');
        file.read(&name[0], nameLength);
        
        size_t typeLength;
        file.read((char*)&typeLength, sizeof(typeLength));
        std::string type(typeLength, '\0');
        file.read(&type[0], typeLength);
        
        columnsNT.push_back({name, type});
        columns[name] = std::vector<ColumnType>(rowCount);
    }

    for (size_t i = 0; i < rowCount; ++i) {
        for (auto& [colName, colData] : columns) {
            ColumnType value;
            char type;
            file.read(&type, sizeof(type));
            if (type == 'i') {
                int temp;
                file.read((char*)&temp, sizeof(temp));
                value = temp;
            } else if (type == 'd') {
                double temp;
                file.read((char*)&temp, sizeof(temp));
                value = temp;
            } else if (type == 's') {
                size_t length;
                file.read((char*)&length, sizeof(length));
                std::string temp(length, '\0');
                file.read(&temp[0], length);
                value = temp;
            }
            colData[i] = value;
        }
    }
}

void Table::addColumn(const std::string& name, const std::string& type) {
    columnsNT.push_back({name, type});
    std::cout << "Column " << name << " of type " << type << " added to " << this->name << "." << std::endl;
}

void Table::updateRow(const std::string& columnName, const std::string& operation, const ColumnType& value) {
    for (size_t i = 0; i < columns[columnName].size(); ++i) {
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, int>) {
                if (operation == "=") {
                    arg = std::get<int>(value);
                } else if (operation == "+") {
                    arg += std::get<int>(value);
                } else if (operation == "-") {
                    arg -= std::get<int>(value);
                }
            } else if constexpr (std::is_same_v<T, double>) {
                if (operation == "=") {
                    arg = std::get<double>(value);
                } else if (operation == "+") {
                    arg += std::get<double>(value);
                } else if (operation == "-") {
                    arg -= std::get<double>(value);
                }
            }
        }, columns[columnName][i]);
    }
}

void Table::updateColumn(const std::string& columnName, const std::string& operation, const ColumnType& value) {
    for (size_t i = 0; i < columns[columnName].size(); ++i) {
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, int>) {
                if (operation == "=") {
                    arg = std::get<int>(value);
                } else if (operation == "+") {
                    arg += std::get<int>(value);
                } else if (operation == "-") {
                    arg -= std::get<int>(value);
                }
            } else if constexpr (std::is_same_v<T, double>) {
                if (operation == "=") {
                    arg = std::get<double>(value);
                } else if (operation == "+") {
                    arg += std::get<double>(value);
                } else if (operation == "-") {
                    arg -= std::get<double>(value);
                }
            }
        }, columns[columnName][i]);
    }
}

void Table::updateColumn(const std::string& columnName, const std::string& operation, const ColumnType& value, const std::string& whereColumn, const std::string& whereOperator, const ColumnType& whereValue) {
    const auto& whereColData = columns.at(whereColumn);
    for (size_t i = 0; i < whereColData.size(); ++i) {
        bool conditionMet = false;
        if (whereOperator == "=") {
            conditionMet = (whereColData[i] == whereValue);
        } else if (whereOperator == "<") {
            conditionMet = (whereColData[i] < whereValue);
        } else if (whereOperator == ">") {
            conditionMet = (whereColData[i] > whereValue);
        }
        if (conditionMet) {
            std::visit([&](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, int>) {
                    if (operation == "=") {
                        arg = std::get<int>(value);
                    } else if (operation == "+") {
                        arg += std::get<int>(value);
                    } else if (operation == "-") {
                        arg -= std::get<int>(value);
                    }
                } else if constexpr (std::is_same_v<T, double>) {
                    if (operation == "=") {
                        arg = std::get<double>(value);
                    } else if (operation == "+") {
                        arg += std::get<double>(value);
                    } else if (operation == "-") {
                        arg -= std::get<double>(value);
                    }
                }
            }, columns[columnName][i]);
        }
    }
}
