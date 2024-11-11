// globals.hpp
#ifndef GLOBALS_HPP
#define GLOBALS_HPP
#include <variant>
#include <fstream>

extern std::ifstream inputFile;
extern std::ofstream outputFile;
using ColumnType = std::variant<int, double, std::string>;
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
    ADD_OP,
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

enum Update_op : int {
    SET,
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    ERROR_UPDATE_OP
};

struct UpdateConfig {
    std::string column;
    Update_op operation;
    ColumnType value;
};

std::ostream& operator<<(std::ostream& os, const ColumnType& column);
/*
struct Condition_parameter{
    Condition_parameter(const BOOL_OP &pre,const string &v1,const string &v2,const string & op);
    BOOL_OP pre_bool_op;
    Column_pos column;
    Compare_sign sign;
    Table_content content;
};
*/
class Condition{
    public:
        Condition(const BOOL_OP &pre,const std::string &v1,const std::string &v2,const std::string & op);
        BOOL_OP pre_bool_op;
        std::string column;
        ColumnType value;
        Compare_sign sign;
};

#endif // GLOBALS_HPP
