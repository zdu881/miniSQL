// globals.cpp
#include "globals.hpp"

std::ifstream inputFile;
std::ofstream outputFile;
int linenumber = 0;
std::ostream& operator<<(std::ostream& os, const ColumnType& column) {
    std::visit([&os](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, int> ) {
            os << arg;
        }else if( std::is_same_v<T, double>){
            //fixed 2 decimal
            os << std::fixed << std::setprecision(2) << arg;
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
    else if(op == "!=") sign = UNEQUAL; // 添加对 != 的解析
    else if(op == "<") sign = SMALLER;
    else if(op == ">") sign = BIGGER;
    else if(op == "<=") sign = LESS_EQUAL;       // 添加对 <= 的解析
    else if(op == ">=") sign = GREATER_EQUAL;    // 添加对 >= 的解析
    else sign = ERROR_COMPARE_SIGN;
}
