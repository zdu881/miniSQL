#include <iostream>
#include "query.hpp"

int main(int argv,char* argc[]){
    //input:: MySQL input.sql output.txt
    if(argv!=3){
        std::cout<<"Please input the correct command"<<std::endl;
        return 0;
    }
    std::cout<<">_< Welcome to MiniSQL"<<std::endl;
    while(1){
        Query query;
        query.getQ();
        query.excQ();
    }
    return 0;
}
