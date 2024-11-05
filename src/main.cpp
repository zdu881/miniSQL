#include <iostream>
#include "../includes/query.hpp"
int main(int argc, char *argv[]) {
    std::cout<<">_< Welcome to MiniSQL"<<std::endl;
    while(1){
        Query query;
        query.getQ();
        query.excQ();
    }
    return 0;
}
