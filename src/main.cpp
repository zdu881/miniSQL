#include <iostream>
#include "query.hpp"

int main() {
    std::cout<<">_< Welcome to MiniSQL"<<std::endl;
    while(1){
        Query query;
        query.getQ();
        query.excQ();
    }
    return 0;
}
