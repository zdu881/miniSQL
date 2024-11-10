// globals.hpp
#ifndef GLOBALS_HPP
#define GLOBALS_HPP
#include <variant>
#include <fstream>

extern std::ifstream inputFile;
extern std::ofstream outputFile;
enum Data_type:int{
    TEXT,
    INTEGER,
    FLOAT,
    CONDITION,
    COLUMN_POS,
    SET_CONFIGS,
    ERROR_TYPE
};

enum Command_type:int{
    CREATE_DATABASE,
    USE_DATABASE,
    CREATE_TABLE,
    DROP_TABLE,
    INSERT_INTO,
    SELECT_FROM,
    SELECT_FROM_INNER_JOIN_ON,
    UPDATE_SET_WHERE,
    DELETE_FROM_WHERE,
    ERROR_COMMAND
};

enum Compare_sign:int{
    EQUAL,
    BIGGER,
    SMALLER,
    ERROR_COMPARE_SIGN
};

enum Compute_op{
    ADD,
    SUB,
    DIV,
    MUT,
    ERROR_COMPUTE_OP
};

enum BOOL_OP:int{
    AND,
    OR,
    ERROR_BOOL_OP
};

/*
struct Condition_parameter{
    Condition_parameter(const BOOL_OP &pre,const string &v1,const string &v2,const string & op);
    BOOL_OP pre_bool_op;
    Column_pos column;
    Compare_sign sign;
    Table_content content;
};
*/

using ColumnType = std::variant<int, double, std::string>;
std::ostream& operator<<(std::ostream& os, const ColumnType& column);
class Condition{
    public:
        Condition(const BOOL_OP &pre,const std::string &v1,const std::string &v2,const std::string & op);
        BOOL_OP pre_bool_op;
        std::string column;
        ColumnType value;
        Compare_sign sign;
};

#endif // GLOBALS_HPP
