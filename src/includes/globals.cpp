// globals.cpp
#include "globals.hpp"

std::ifstream inputFile;
std::ofstream outputFile;
std::ostream& operator<<(std::ostream& os, const ColumnType& column) {
    std::visit([&os](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double>) {
            os << arg;
        } else if constexpr (std::is_same_v<T, std::string>) {
            os << arg;
        }
    }, column);
    return os;
}

Condition::Condition(const BOOL_OP &pre,const std::string &v1,const std::string &v2,const std::string & op){
    pre_bool_op = pre;
    column = v1;
    value = v2;
    if(op == "=") sign = EQUAL;
    else if(op == "<") sign = SMALLER;
    else if(op == ">") sign = BIGGER;
    else sign = ERROR_COMPARE_SIGN;
}