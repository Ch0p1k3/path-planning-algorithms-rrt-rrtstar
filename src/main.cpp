#include <iostream>
#include "algorithm.hpp"
#include "geometry.hpp"
#include "obstacle.hpp"
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
    Algorithm algo(argv[1]);
    std::cout << algo.getSearchType() << '\n';
    std::cout << algo.getNumberOfIterations() << '\n';
    Obstacle ob(m);
    std::cout << ob.freeObstacle(Geometry::Point(0, 0), Geometry::Point(3, 0)) << '\n';
    std::cout << ob.freeObstacle(Geometry::Point(17, 19.5), Geometry::Point(20, 19.5)) << '\n';
}