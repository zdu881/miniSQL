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
    size_t rowCount = this->columns.begin()->second.size();
        for (const auto& column : columns) {
        outputFile << column << " ";
    }
    for (size_t i = 0; i < rowCount; ++i) {
        for (const auto& column : columns) {
            if (this->columns.find(column) == this->columns.end()) {
                std::cerr << "Column " << column << " does not exist in table " << name << std::endl;
                continue;
            }

            const auto& colData = this->columns.at(column);

            if (this->getColumnType(column) == INTEGER || this->getColumnType(column) == FLOAT) {
                outputFile << " " << colData[i] << " " ;
            } else if (this->getColumnType(column) == TEXT) {
                outputFile << " \"" << std::get<std::string>(colData[i]) << "\" " ;
            } else {
                std::cerr << "ERROR: Unknown column type for column " << column ;
            }
        }
        outputFile << std::endl;
    }
    std::cout << "QueryTable" << std::endl;
}



void Table::queryTable(const std::vector<std::string>& columns, const std::vector<Condition>& conditions) const {
    // std::cout 替换为 outputFile
    // 输出列名
    outputFile << "-----------------------------" << std::endl;
    for (const auto& column : columns) {
        outputFile << column << " ";
    }
    outputFile << std::endl;
    std::cout << "Table " << name << " contents with conditions:" << std::endl;

    // 输出符合条件的行
    for (size_t i = 0; i < this->columns.begin()->second.size(); ++i) {
        bool match = true;
        for (const auto& condition : conditions) {
            if (this->columns.find(condition.column) == this->columns.end()) {
                std::cerr << "Column " << condition.column << " does not exist in table " << name << std::endl;
                match = false;
                break;
            }

            const auto& colData = this->columns.at(condition.column);
            Data_type colType = getColumnType(condition.column);
            if (colType == ERROR_TYPE) {
                outputFile << "Column " << condition.column << " does not exist." << std::endl;
                match = false;
                break;
            }

            bool conditionMatch = std::visit([&condition, colType](const ColumnType& arg) -> bool {
                try {
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
                } catch (const std::bad_variant_access&) {
                    std::cerr << "Bad variant access for column " << condition.column << std::endl;
                    return false;
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                    return false;
                }
            }, colData[i]);

            if (!conditionMatch) {
                match = false;
                break;
            }
        }

        if (match) {
            for (const auto& column : columns) {
                if (this->columns.find(column) == this->columns.end()) {
                    std::cerr << "Column " << column << " does not exist in table " << name << std::endl;
                    continue;
                }

                const auto& colData = this->columns.at(column);
                std::visit([](const auto& arg) {
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
            // 添加条件匹配逻辑
            bool conditionMatch = std::visit([&condition, colType](const ColumnType& arg) -> bool {
                try {
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
                } catch (const std::bad_variant_access&) {
                    std::cerr << "Bad variant access for column " << condition.column << std::endl;
                    return false;
                }
                return false;
            }, colData[i]);

            if (!conditionMatch) {
                deleteRow = false;
                break;
            }
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
void Table::updateRow(const std::vector<UpdateConfig>& setConfigs, const std::vector<Condition>& conditions) {
    std::vector<size_t> rowsToUpdate;
    std::cout<<"conditions.size()"<<conditions.size()<<std::endl;
    //0!!!!!!!!!!!!!!!!
    for (size_t i = 0; i < this->columns.begin()->second.size(); ++i) {
        bool updateRow = true;
        for (const auto& condition : conditions) {
            if (this->columns.find(condition.column) == this->columns.end()) {
                std::cerr << "Column " << condition.column << " does not exist in table " << name << std::endl;
                updateRow = false;
                break;
            }

            const auto& colData = this->columns.at(condition.column);
            Data_type colType = getColumnType(condition.column);
            if (colType == ERROR_TYPE) {
                std::cerr << "Column " << condition.column << " does not exist." << std::endl;
                updateRow = false;
                break;
            }
            std::cout<<"BEFORE CONDITION"<<std::endl;
            bool conditionMatch = std::visit([&condition, colType](const ColumnType& arg) -> bool {
                try {
                    std::cout<<"value"<<std::get<std::string>(condition.value)<<std::endl;
                    std::cout<<"arg"<<arg<<std::endl;  
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
                } catch (const std::bad_variant_access&) {
                    std::cerr << "Bad variant access for column " << condition.column << std::endl;
                    return false;
                }
                return false;
            }, colData[i]);

            if (!conditionMatch) {
                updateRow = false;
                break;
            }
        }
        if (updateRow) {
            rowsToUpdate.push_back(i);
        }
    }

    std::cout << "rowsToUpdate.size() " << rowsToUpdate.size() << std::endl;
    //outputsetConfigs
    std::cout<<"setConfigs"<<std::endl;
    for(const auto& config : setConfigs) std::cout << config.column << " " << config.value << std::endl;
    for (auto& [columnName, columnData] : columns) {
        Data_type colType = getColumnType(columnName);
        for (size_t i = 0; i < rowsToUpdate.size(); ++i) {
            auto it = std::find_if(setConfigs.begin(), setConfigs.end(), [&columnName](const UpdateConfig& config) { return config.column == columnName; });
            if (it != setConfigs.end()) {
                switch(it->operation) {
                    case SET:
                        if(colType == INTEGER) {
                            columnData[rowsToUpdate[i]] = std::stoi(std::get<std::string>(it->value));
                        } else if(colType == FLOAT) {
                            columnData[rowsToUpdate[i]] = std::stod(std::get<std::string>(it->value));
                        } else if(colType == TEXT) {
                            columnData[rowsToUpdate[i]] = it->value;
                        }
                        break;
                    case ADD:
                        if(colType == INTEGER) {
                            columnData[rowsToUpdate[i]] = std::get<int>(columnData[rowsToUpdate[i]]) + std::stoi(std::get<std::string>(it->value));
                        } else if(colType == FLOAT) {
                            columnData[rowsToUpdate[i]] = std::get<double>(columnData[rowsToUpdate[i]]) + std::stod(std::get<std::string>(it->value));
                        } else {
                            std::cerr << "Unsupported operation ADD on column " << columnName << std::endl;
                        }
                        break;
                    case SUBTRACT:
                        if(colType == INTEGER) {
                            columnData[rowsToUpdate[i]] = std::get<int>(columnData[rowsToUpdate[i]]) - std::stoi(std::get<std::string>(it->value));
                        } else if(colType == FLOAT) {
                            columnData[rowsToUpdate[i]] = std::get<double>(columnData[rowsToUpdate[i]]) - std::stod(std::get<std::string>(it->value));
                        } else {
                            std::cerr << "Unsupported operation SUBTRACT on column " << columnName << std::endl;
                        }
                        break;
                    case MULTIPLY:
                        if(colType == INTEGER) {
                            columnData[rowsToUpdate[i]] = std::get<int>(columnData[rowsToUpdate[i]]) * std::stoi(std::get<std::string>(it->value));
                        } else if(colType == FLOAT) {
                            columnData[rowsToUpdate[i]] = std::get<double>(columnData[rowsToUpdate[i]]) * std::stod(std::get<std::string>(it->value));
                        } else {
                            std::cerr << "Unsupported operation MULTIPLY on column " << columnName << std::endl;
                        }
                        break;
                    case DIVIDE:
                        if(colType == INTEGER) {
                            columnData[rowsToUpdate[i]] = std::get<int>(columnData[rowsToUpdate[i]]) / std::stoi(std::get<std::string>(it->value));
                        } else if(colType == FLOAT) {
                            columnData[rowsToUpdate[i]] = std::get<double>(columnData[rowsToUpdate[i]]) / std::stod(std::get<std::string>(it->value));
                        } else {
                            std::cerr << "Unsupported operation DIVIDE on column " << columnName << std::endl;
                        }
                        break;
                    default:
                        std::cerr << "Unsupported update operation." << std::endl;
                        break;
                }
            }
        }
    }

    std::cout << rowsToUpdate.size() << " rows updated in " << name << "." << std::endl;
}
void Table::save(std::ofstream& file) const {
    // 保存表名
    size_t nameLength = name.size();
    file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
    file.write(name.c_str(), nameLength);

    // 保存列信息
    size_t columnCount = columnsNT.size();
    file.write(reinterpret_cast<const char*>(&columnCount), sizeof(columnCount));
    for (const auto& [colName, colType] : columnsNT) {
        size_t colNameLength = colName.size();
        file.write(reinterpret_cast<const char*>(&colNameLength), sizeof(colNameLength));
        file.write(colName.c_str(), colNameLength);
        file.write(reinterpret_cast<const char*>(&colType), sizeof(colType));
    }

    // 保存数据行
    size_t rowCount = columns.empty() ? 0 : columns.begin()->second.size();
    file.write(reinterpret_cast<const char*>(&rowCount), sizeof(rowCount));
    for (size_t i = 0; i < rowCount; ++i) {
        for (const auto& [colName, colData] : columnsNT) {
            const auto& value = columns.at(colName)[i];
            std::visit([&file](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, int>) {
                    file.write(reinterpret_cast<const char*>(&arg), sizeof(arg));
                } else if constexpr (std::is_same_v<T, double>) {
                    file.write(reinterpret_cast<const char*>(&arg), sizeof(arg));
                } else if constexpr (std::is_same_v<T, std::string>) {
                    size_t strLength = arg.size();
                    file.write(reinterpret_cast<const char*>(&strLength), sizeof(strLength));
                    file.write(arg.c_str(), strLength);
                }
            }, value);
        }
    }
}

void Table::load(std::ifstream& file) {
    // 读取表名
    size_t nameLength;
    file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
    name.resize(nameLength);
    file.read(&name[0], nameLength);

    // 读取列信息
    size_t columnCount;
    file.read(reinterpret_cast<char*>(&columnCount), sizeof(columnCount));
    columnsNT.clear();
    columns.clear();
    for (size_t i = 0; i < columnCount; ++i) {
        size_t colNameLength;
        file.read(reinterpret_cast<char*>(&colNameLength), sizeof(colNameLength));
        std::string colName(colNameLength, '\0');
        file.read(&colName[0], colNameLength);
        Data_type colType;
        file.read(reinterpret_cast<char*>(&colType), sizeof(colType));
        columnsNT.emplace_back(colName, colType);
        columns[colName] = std::vector<ColumnType>();
    }

    // 初始化列数据
    for (auto& [colName, colType] : columnsNT) {
        switch (colType) {
            case INTEGER:
                columns[colName].emplace_back(0);
                break;
            case FLOAT:
                columns[colName].emplace_back(0.0);
                break;
            case TEXT:
                columns[colName].emplace_back(std::string());
                break;
            default:
                // 处理错误类型
                break;
        }
    }

    // 读取数据行
    size_t rowCount;
    file.read(reinterpret_cast<char*>(&rowCount), sizeof(rowCount));
    for (size_t i = 0; i < rowCount; ++i) {
        for (auto& [colName, colType] : columnsNT) {
            ColumnType value;
            switch (colType) {
                case INTEGER: {
                    int intVal;
                    file.read(reinterpret_cast<char*>(&intVal), sizeof(intVal));
                    value = intVal;
                    break;
                }
                case FLOAT: {
                    double doubleVal;
                    file.read(reinterpret_cast<char*>(&doubleVal), sizeof(doubleVal));
                    value = doubleVal;
                    break;
                }
                case TEXT: {
                    size_t strLength;
                    file.read(reinterpret_cast<char*>(&strLength), sizeof(strLength));
                    std::string strVal(strLength, '\0');
                    file.read(&strVal[0], strLength);
                    value = strVal;
                    break;
                }
                default:
                    // 处理错误类型
                    break;
            }
            columns[colName].push_back(value);
        }
    }
}

void Table::addColumn(const std::string& name, const Data_type& type) {
    columnsNT.push_back({name, type});
    std::cout << "Column " << name << " of type " << type << " added to " << this->name << "." << std::endl;
}
