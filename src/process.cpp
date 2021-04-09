#include "process.hpp"

void RRTAlgorithm::launch(const Map& map, const Algorithm& algo)
{
    size_t countOfEdges = 0;
    RRT rrt(map, algo);
    Geometry::Point finish = rrt.getFinish();
    Tree::Node *finishNode = nullptr;
    const double EPS = rrt.getEps() * rrt.getEps();
    const size_t numberOfIter = rrt.getNumberOfIterations();
    auto time = std::chrono::steady_clock::now();
    for (size_t it = 0; it < numberOfIter; ++it) {
        Geometry::Point xRand = rrt.getRandomPoint();
        Tree::Node *xNearest = rrt.getNearest(xRand);
        Geometry::Point xNew = rrt.steer(xNearest->point, xRand);
        if (rrt.obstacleFree(xNearest->point, xNew)) {
            Tree::Node *newNode = rrt.insertEdge(xNearest, xNew);
            if (newNode) {
                ++countOfEdges;
                if (Geometry::euclideanMetric(newNode->point, finish) <= EPS) {
                    if (!finishNode) {
                        finishNode = newNode;
                    } else {
                        if (finishNode->distance > newNode->distance) {
                            finishNode = newNode;
                        }
                    }
                    break;
                }
            }
        }
    }
    double timeRes = std::chrono::duration<double>(std::chrono::steady_clock::now() - time).count();
    std::cout.precision(8);
    std::cout << std::fixed;
    std::cout << "Time: " << timeRes << "\nCount of edges: " << countOfEdges << '\n';
    if (!finishNode) {
        std::cout << "Path not found.\n";
    } else {
        std::cout << "Result distance: " << finishNode->distance << '\n';
        Tree::Node *tmp = finishNode;
        std::vector<Tree::Node *> res;
        while (tmp) {
            res.push_back(tmp);
            tmp = tmp->parent;
        }
        std::cout << "Path:\n";
        std::reverse(res.begin(), res.end());
        for (size_t i = 0; i < res.size(); ++i) {
            if (!i) {
                std::cout << res[i]->point;
            } else {
                std::cout << "\n" << res[i]->point;
            }
        }
    }
    // std::cout << "\nTree:\n";
    // rrt.printTree();
}

void RRTAlgorithm::launchWithVis(const Map& map, const Algorithm& algo)
{
    
    size_t countOfEdges = 0;
    RRT rrt(map, algo);
    Geometry::Point start = rrt.getStart();
    Geometry::Point finish = rrt.getFinish();
    Tree::Node *finishNode = nullptr;
    const double EPS = rrt.getEps() * rrt.getEps();
    const size_t numberOfIter = rrt.getNumberOfIterations();
    size_t height = map.getMapHeight();
    size_t width = map.getMapWidth();
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(desktop.width - 100, desktop.height - 100, desktop.bitsPerPixel), "Algorithm", sf::Style::Close | sf::Style::Titlebar, settings);
    sf::View view(sf::FloatRect(0., 0., (float)width + 1, (float)height + 1));
    window.setView(view);
    // window.setFramerateLimit(10000);
    std::vector<sf::Shape> obst;
    bool isReady = false;
    auto time = std::chrono::steady_clock::now();
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        if (!isReady) {
            window.clear(sf::Color::White);
            for (size_t i = 0; i < height && window.isOpen(); ++i) {
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }
                }
                for (size_t j = 0; j < width && window.isOpen(); ++j) {
                    while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed) {
                            window.close();
                        }
                    }
                    if (map[i][j]) {
                        if (window.isOpen()) {
                            sf::RectangleShape quad(sf::Vector2f(1., 1.));
                            quad.setFillColor(sf::Color(0, 0, 125));
                            quad.setPosition((float)j, (float)i);
                            window.draw(quad);
                        }
                    }
                }
            }
            sf::CircleShape startCircle(((double)height / 100) * 0.5);
            startCircle.setOrigin(startCircle.getRadius(), startCircle.getRadius());
            startCircle.setPosition(sf::Vector2f(start.x, start.y));
            startCircle.setFillColor(sf::Color(0, 106, 0));
            sf::CircleShape finishCircle(rrt.getEps()); // width / desksf::RenderWindow window(sf::VideoMode(desktop.width - 100, desktop.height - 100 * 3
            finishCircle.setOrigin(finishCircle.getRadius(), finishCircle.getRadius());
            finishCircle.setPosition(sf::Vector2f(finish.x, finish.y));
            finishCircle.setFillColor(sf::Color::Transparent);
            finishCircle.setOutlineThickness(finishCircle.getRadius() / 100 * 40);
            finishCircle.setOutlineColor(sf::Color(255, 169, 0));
            window.draw(startCircle);
            window.draw(finishCircle);
            window.display();
            auto time = std::chrono::steady_clock::now();
            double resTime = 0;
            auto tmp = std::chrono::steady_clock::now();
            for (size_t it = 0; (it < numberOfIter); ++it) {
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }
                }
                Geometry::Point xRand = rrt.getRandomPoint();
                Tree::Node *xNearest = rrt.getNearest(xRand);
                Geometry::Point xNew = rrt.steer(xNearest->point, xRand);
                if (rrt.obstacleFree(xNearest->point, xNew)) {
                    Tree::Node *newNode = rrt.insertEdge(xNearest, xNew);
                    if (newNode) {
                        ++countOfEdges;
                        tmp = std::chrono::steady_clock::now();
                        if (window.isOpen()) {
                            sf::CircleShape nodeCircle(((double)height / 100) * 0.1);
                            nodeCircle.setFillColor(sf::Color::Black);
                            nodeCircle.setOrigin(nodeCircle.getRadius(), nodeCircle.getRadius());
                            nodeCircle.setPosition(sf::Vector2f(xNew.x, xNew.y));
                            window.draw(nodeCircle);
                            sf::Vertex line[] = {
                                sf::Vertex(sf::Vector2f(xNearest->point.x, xNearest->point.y)),
                                sf::Vertex(sf::Vector2f(xNew.x, xNew.y)),
                            };
                            line[0].color = sf::Color::Black;
                            line[1].color = sf::Color::Black;
                            window.draw(line, 2, sf::Lines);
                            window.display();
                        }
                        resTime -= std::chrono::duration<double>(std::chrono::steady_clock::now() - tmp).count();
                        if (Geometry::euclideanMetric(newNode->point, finish) <= EPS) {
                            if (!finishNode) {
                                finishNode = newNode;
                            } else {
                                if (finishNode->distance > newNode->distance) {
                                    finishNode = newNode;
                                }
                            }
                            break;
                        }
                    }
                } else {
                    tmp = std::chrono::steady_clock::now();
                    if (window.isOpen()) {
                        sf::CircleShape nodeCircle(((double)height / 100) * 0.1);
                        nodeCircle.setFillColor(sf::Color::Red);
                        nodeCircle.setOrigin(nodeCircle.getRadius(), nodeCircle.getRadius());
                        nodeCircle.setPosition(sf::Vector2f(xNew.x, xNew.y));
                        window.draw(nodeCircle);
                        sf::Vertex line[] = {
                            sf::Vertex(sf::Vector2f(xNearest->point.x, xNearest->point.y)),
                            sf::Vertex(sf::Vector2f(xNew.x, xNew.y)),
                        };
                        line[0].color = sf::Color::Black;
                        line[1].color = sf::Color::Red;
                        window.draw(line, 2, sf::Lines);
                        window.display();
                    }
                    resTime -= std::chrono::duration<double>(std::chrono::steady_clock::now() - tmp).count();
                }
            }
            resTime += std::chrono::duration<double>(std::chrono::steady_clock::now() - time).count();
            std::cout.precision(8);
            std::cout << std::fixed;
            std::cout << "Time: " << resTime << '\n';
            std::cout << "Count of edges: " << countOfEdges << '\n';
            if (!finishNode) {
                std::cout << "Path not found.\n";
            } else {
                std::cout << "Result distance: " << finishNode->distance << '\n';
                Tree::Node *tmp = finishNode;
                std::vector<Tree::Node *> res;
                while (tmp) {
                    while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed) {
                            window.close();
                        }
                    }
                    if (window.isOpen() && !res.empty()) {
                        sf::CircleShape nodeCircle(((double)height / 100) * 0.2);
                        nodeCircle.setFillColor(sf::Color(255, 169, 0));
                        nodeCircle.setOrigin(nodeCircle.getRadius(), nodeCircle.getRadius());
                        nodeCircle.setPosition(sf::Vector2f(tmp->point.x, tmp->point.y));
                        window.draw(nodeCircle);
                        nodeCircle.setPosition(sf::Vector2f(res.back()->point.x, res.back()->point.y));
                        window.draw(nodeCircle);
                        sf::RectangleShape line2(sf::Vector2f(std::sqrt(Geometry::euclideanMetric(res.back()->point, tmp->point)), (double)height / 100 * 0.2));
                        line2.rotate(std::atan2(res.back()->point.y - tmp->point.y, res.back()->point.x - tmp->point.x) / M_PI * 180);
                        line2.setOrigin(0, ((double)height / 100 * 0.2) / 2);
                        line2.setPosition(tmp->point.x, tmp->point.y);
                        line2.setFillColor(sf::Color(255, 169, 0));
                        window.draw(line2);
                    }
                    res.push_back(tmp);
                    tmp = tmp->parent;
                }
                std::cout << "Path:\n";
                std::reverse(res.begin(), res.end());
                for (size_t i = 0; i < res.size(); ++i) {
                    if (!i) {
                        std::cout << res[i]->point;
                    } else {
                        std::cout << "\n" << res[i]->point;
                    }
                }
            }
            window.draw(startCircle);
            window.draw(finishCircle);
            window.display();
            isReady = true;
        }
    }
}

void RRTAlgorithm::launchWithVisAfter(const Map& map, const Algorithm& algo)
{
    size_t countOfEdges = 0;
    RRT rrt(map, algo);
    Geometry::Point finish = rrt.getFinish();
    Geometry::Point start = rrt.getStart();
    Tree::Node *finishNode = nullptr;
    const double EPS = rrt.getEps() * rrt.getEps();
    const size_t numberOfIter = rrt.getNumberOfIterations();
    auto time = std::chrono::steady_clock::now();
    for (size_t it = 0; it < numberOfIter; ++it) {
        Geometry::Point xRand = rrt.getRandomPoint();
        Tree::Node *xNearest = rrt.getNearest(xRand);
        Geometry::Point xNew = rrt.steer(xNearest->point, xRand);
        if (rrt.obstacleFree(xNearest->point, xNew)) {
            Tree::Node *newNode = rrt.insertEdge(xNearest, xNew);
            if (newNode) {
                ++countOfEdges;
                if (Geometry::euclideanMetric(newNode->point, finish) <= EPS) {
                    if (!finishNode) {
                        finishNode = newNode;
                    } else {
                        if (finishNode->distance > newNode->distance) {
                            finishNode = newNode;
                        }
                    }
                    break;
                }
            }
        }
    }
    double resTime = std::chrono::duration<double>(std::chrono::steady_clock::now() - time).count();
    std::cout.precision(8);
    std::cout << std::fixed;
    std::cout << "Time: " << resTime << '\n';
    std::cout << "Count of edges: " << countOfEdges << '\n';
    size_t height = map.getMapHeight();
    size_t width = map.getMapWidth();
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(desktop.width - 100, desktop.height - 100, desktop.bitsPerPixel), "Algorithm", sf::Style::Close | sf::Style::Titlebar, settings);
    sf::View view(sf::FloatRect(0., 0., (float)width + 1, (float)height + 1));
    window.setView(view);
    std::vector<sf::Shape> obst;
    bool isReady = false;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        if (!isReady) {
            window.clear(sf::Color::White);
            for (size_t i = 0; i < height && window.isOpen(); ++i) {
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }
                }
                for (size_t j = 0; j < width && window.isOpen(); ++j) {
                    while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed) {
                            window.close();
                        }
                    }
                    if (map[i][j]) {
                        if (window.isOpen()) {
                            sf::RectangleShape quad(sf::Vector2f(1., 1.));
                            quad.setFillColor(sf::Color(0, 0, 125));
                            quad.setPosition((float)j, (float)i);
                            window.draw(quad);
                        }
                    }
                }
            }
            sf::CircleShape startCircle(((double)height / 100) * 0.5);
            startCircle.setOrigin(startCircle.getRadius(), startCircle.getRadius());
            startCircle.setPosition(sf::Vector2f(start.x, start.y));
            startCircle.setFillColor(sf::Color(0, 106, 0));
            sf::CircleShape finishCircle(rrt.getEps()); // width / desksf::RenderWindow window(sf::VideoMode(desktop.width - 100, desktop.height - 100 * 3
            finishCircle.setOrigin(finishCircle.getRadius(), finishCircle.getRadius());
            finishCircle.setPosition(sf::Vector2f(finish.x, finish.y));
            finishCircle.setFillColor(sf::Color::Transparent);
            finishCircle.setOutlineThickness(finishCircle.getRadius() / 100 * 40);
            finishCircle.setOutlineColor(sf::Color(255, 169, 0));
            rrt.drawTree(window);
            if (finishNode) {
                Tree::Node *tmp = finishNode;
                while (tmp) {
                    if (tmp->parent) {
                        sf::CircleShape nodeCircle(((double)height / 100) * 0.2);
                        nodeCircle.setFillColor(sf::Color(255, 169, 0));
                        nodeCircle.setOrigin(nodeCircle.getRadius(), nodeCircle.getRadius());
                        nodeCircle.setPosition(sf::Vector2f(tmp->point.x, tmp->point.y));
                        window.draw(nodeCircle);
                        nodeCircle.setPosition(sf::Vector2f(tmp->parent->point.x, tmp->parent->point.y));
                        window.draw(nodeCircle);
                        sf::RectangleShape line2(sf::Vector2f(std::sqrt(Geometry::euclideanMetric(tmp->parent->point, tmp->point)), (double)height / 100 * 0.2));
                        line2.rotate(std::atan2(tmp->parent->point.y - tmp->point.y, tmp->parent->point.x - tmp->point.x) / M_PI * 180);
                        line2.setOrigin(0, ((double)height / 100 * 0.2) / 2);
                        line2.setPosition(tmp->point.x, tmp->point.y);
                        line2.setFillColor(sf::Color(255, 169, 0));
                        window.draw(line2);
                    }
                    tmp = tmp->parent;
                }
            }
            window.draw(startCircle);
            window.draw(finishCircle);
            window.display();
            if (!finishNode) {
                std::cout << "Path not found.\n";
            } else {
                std::cout << "Result distance: " << finishNode->distance << '\n';
                Tree::Node *tmp = finishNode;
                std::vector<Tree::Node *> res;
                while (tmp) {
                    res.push_back(tmp);
                    tmp = tmp->parent;
                }
                std::reverse(res.begin(), res.end());
                for (size_t i = 0; i < res.size(); ++i) {
                    if (!i) {
                        std::cout << res[i]->point;
                    } else {
                        std::cout << "\n" << res[i]->point;
                    }
                }
            }
        }
        isReady = true;
    }
}

void RRTAlgorithm::launchWithVisAfterWithoutTree(const Map& map, const Algorithm& algo)
{
    size_t countOfEdges = 0;
    RRT rrt(map, algo);
    Geometry::Point finish = rrt.getFinish();
    Geometry::Point start = rrt.getStart();
    Tree::Node *finishNode = nullptr;
    const double EPS = rrt.getEps() * rrt.getEps();
    const size_t numberOfIter = rrt.getNumberOfIterations();
    auto time = std::chrono::steady_clock::now();
    for (size_t it = 0; it < numberOfIter; ++it) {
        Geometry::Point xRand = rrt.getRandomPoint();
        Tree::Node *xNearest = rrt.getNearest(xRand);
        Geometry::Point xNew = rrt.steer(xNearest->point, xRand);
        if (rrt.obstacleFree(xNearest->point, xNew)) {
            Tree::Node *newNode = rrt.insertEdge(xNearest, xNew);
            if (newNode) {
                ++countOfEdges;
                if (Geometry::euclideanMetric(newNode->point, finish) <= EPS) {
                    if (!finishNode) {
                        finishNode = newNode;
                    } else {
                        if (finishNode->distance > newNode->distance) {
                            finishNode = newNode;
                        }
                    }
                    break;
                }
            }
        }
    }
    double resTime = std::chrono::duration<double>(std::chrono::steady_clock::now() - time).count();
    std::cout.precision(8);
    std::cout << std::fixed;
    std::cout << "Time: " << resTime << '\n';
    std::cout << "Count of edges: " << countOfEdges << '\n';
    size_t height = map.getMapHeight();
    size_t width = map.getMapWidth();
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(desktop.width - 100, desktop.height - 100, desktop.bitsPerPixel), "Algorithm", sf::Style::Close | sf::Style::Titlebar, settings);
    sf::View view(sf::FloatRect(0., 0., (float)width + 1, (float)height + 1));
    window.setView(view);
    std::vector<sf::Shape> obst;
    bool isReady = false;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        if (!isReady) {
            window.clear(sf::Color::White);
            for (size_t i = 0; i < height && window.isOpen(); ++i) {
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }
                }
                for (size_t j = 0; j < width && window.isOpen(); ++j) {
                    while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed) {
                            window.close();
                        }
                    }
                    if (map[i][j]) {
                        if (window.isOpen()) {
                            sf::RectangleShape quad(sf::Vector2f(1., 1.));
                            quad.setFillColor(sf::Color(0, 0, 125));
                            quad.setPosition((float)j, (float)i);
                            window.draw(quad);
                        }
                    }
                }
            }
            sf::CircleShape startCircle(((double)height / 100) * 0.5);
            startCircle.setOrigin(startCircle.getRadius(), startCircle.getRadius());
            startCircle.setPosition(sf::Vector2f(start.x, start.y));
            startCircle.setFillColor(sf::Color(0, 106, 0));
            sf::CircleShape finishCircle(rrt.getEps()); // width / desksf::RenderWindow window(sf::VideoMode(desktop.width - 100, desktop.height - 100 * 3
            finishCircle.setOrigin(finishCircle.getRadius(), finishCircle.getRadius());
            finishCircle.setPosition(sf::Vector2f(finish.x, finish.y));
            finishCircle.setFillColor(sf::Color::Transparent);
            finishCircle.setOutlineThickness(finishCircle.getRadius() / 100 * 40);
            finishCircle.setOutlineColor(sf::Color(255, 169, 0));
            if (finishNode) {
                Tree::Node *tmp = finishNode;
                while (tmp) {
                    if (tmp->parent) {
                        sf::CircleShape nodeCircle(((double)height / 100) * 0.2);
                        nodeCircle.setFillColor(sf::Color(255, 169, 0));
                        nodeCircle.setOrigin(nodeCircle.getRadius(), nodeCircle.getRadius());
                        nodeCircle.setPosition(sf::Vector2f(tmp->point.x, tmp->point.y));
                        window.draw(nodeCircle);
                        nodeCircle.setPosition(sf::Vector2f(tmp->parent->point.x, tmp->parent->point.y));
                        window.draw(nodeCircle);
                        sf::RectangleShape line2(sf::Vector2f(std::sqrt(Geometry::euclideanMetric(tmp->parent->point, tmp->point)), (double)height / 100 * 0.2));
                        line2.rotate(std::atan2(tmp->parent->point.y - tmp->point.y, tmp->parent->point.x - tmp->point.x) / M_PI * 180);
                        line2.setOrigin(0, ((double)height / 100 * 0.2) / 2);
                        line2.setPosition(tmp->point.x, tmp->point.y);
                        line2.setFillColor(sf::Color(255, 169, 0));
                        window.draw(line2);
                    }
                    tmp = tmp->parent;
                }
            }
            window.draw(startCircle);
            window.draw(finishCircle);
            window.display();
            if (!finishNode) {
                std::cout << "Path not found.\n";
            } else {
                std::cout << "Result distance: " << finishNode->distance << '\n';
                Tree::Node *tmp = finishNode;
                std::vector<Tree::Node *> res;
                while (tmp) {
                    res.push_back(tmp);
                    tmp = tmp->parent;
                }
                std::reverse(res.begin(), res.end());
                for (size_t i = 0; i < res.size(); ++i) {
                    if (!i) {
                        std::cout << res[i]->point;
                    } else {
                        std::cout << "\n" << res[i]->point;
                    }
                }
            }
        }
        isReady = true;
    }
}