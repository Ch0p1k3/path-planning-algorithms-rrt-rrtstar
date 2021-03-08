#include <iostream>
#include "geometry.hpp"
#include "map.hpp"

int main(int argc, char *argv[])
{
    if(argc < 2) {
        std::cout<<"Error! Pathfinding task file (XML) is not specified!"<<std::endl;
        return 0;
    }
    Map m(argv[1]);
}