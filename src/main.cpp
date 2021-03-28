#include <iostream>
#include "algorithm.hpp"
#include "geometry.hpp"
#include "obstacle.hpp"
#include "map.hpp"
#include "tree.hpp"
#include "RRT.hpp"
#include "process.hpp"
#include "lib/SFML/include/SFML/Graphics.hpp"

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
    // for (size_t i = 0; i < m.getMapHeight(); ++i) {
    //     for (size_t j = 0; j < m.getMapWidth(); ++j) {
    //         std::cout << m[i][j] << ' ';
    //     }
    //     std::cout << '\n';
    // }
    Algorithm algo(argv[1]);
    std::cout << algo.getSearchType() << '\n';
    std::cout << algo.getNumberOfIterations() << '\n';
    Obstacle ob(m);
    std::cout << ob.obstacleFree(Geometry::Point(0, 0), Geometry::Point(3, 0)) << '\n';
    std::cout << ob.obstacleFree(Geometry::Point(17, 19.5), Geometry::Point(20, 19.5)) << '\n';
    Tree tr(Geometry::Point(0, 0));
    auto nearest = tr.getNearest(Geometry::Point{1, 1});
    std::cout << nearest->point << ' ' << nearest->distance << '\n';
    tr.insert(nearest, {1, 1});
    nearest = tr.getNearest(Geometry::Point{1.25, 1.25});
    std::cout << nearest->point << ' ' << nearest->distance << '\n';
    tr.insert(nearest, {1.25, 1.25});
    tr.insert(nearest, {1.1, 1.1});
    tr.insert(nearest, {1.04, 1.04});
    tr.printTree();
    std::cout << "-----------------------\n";
    std::cout << "-----------------------\n";
    RRTAlgorithm::launchWithVirtAfterWithoutTree(m, algo);
}