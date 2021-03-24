#include "process.hpp"

void RRTAlgorithm::launch(const Map& map, const Algorithm& algo)
{
    RRT rrt(map, algo);
    Geometry::Point finish = rrt.getFinish();
    std::shared_ptr<Tree::Node> finishNode;
    const double EPS = CI_STEP_SIZE;
    for (size_t it = 0; it < rrt.getNumberOfIterations(); ++it) {
        Geometry::Point xRand = rrt.getRandomPoint();
        std::shared_ptr<Tree::Node> xNearest = rrt.getNearest(xRand);
        Geometry::Point xNew = rrt.steer(xNearest->point, xRand);
        if (rrt.obstacleFree(xNearest->point, xNew)) {
            std::shared_ptr<Tree::Node> newNode = rrt.insertEdge(xNearest, xNew);
            if (newNode) {
                if (Geometry::euclideanMetric(newNode->point, finish) <= EPS) {
                    if (!finishNode) {
                        finishNode = newNode;
                    } else {
                        if (finishNode->distance > newNode->distance) {
                            finishNode = newNode;
                        }
                    }
                }
            }
        }
    }
    freopen("output.txt", "w", stdout);
    if (!finishNode) {
        std::cout << "Path not found\n";
    } else {
        std::cout << "Result distance: " << finishNode->distance << '\n';
        auto tmp = finishNode;
        std::vector<std::shared_ptr<Tree::Node>> res;
        while (tmp) {
            res.push_back(tmp);
            tmp = tmp->parent;
        }
        std::reverse(res.begin(), res.end());
        for (size_t i = 0; i < res.size(); ++i) {
            if (!i) {
                std::cout << res[i]->point;
            } else {
                std::cout << "->" << res[i]->point;
            }
        }
    }
    std::cout << "Tree:\n";
    rrt.printTree();
}