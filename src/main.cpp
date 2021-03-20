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
    std::cout << m.getStart() << '\n';
    std::cout << m.getFinish() << '\n';
    std::cout << m.getMapHeight() << '\n';
    std::cout << m.getMapWidth() << '\n';
    for (size_t i = 0; i < m.getMapHeight(); ++i) {
        for (size_t j = 0; j < m.getMapWidth(); ++j) {
            std::cout << m[i][j] << ' ';
        }
        std::cout << '\n';
    }
}