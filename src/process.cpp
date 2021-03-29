#include "process.hpp"

void RRTAlgorithm::launch(const Map& map, const Algorithm& algo)
{
    RRT rrt(map, algo);
    Geometry::Point finish = rrt.getFinish();
    Tree::Node *finishNode = nullptr;
    const double EPS = CI_STEP_SIZE;
    const size_t numberOfIter = rrt.getNumberOfIterations();
    for (size_t it = 0; it < numberOfIter; ++it) {
        Geometry::Point xRand = rrt.getRandomPoint();
        Tree::Node *xNearest = rrt.getNearest(xRand);
        Geometry::Point xNew = rrt.steer(xNearest->point, xRand);
        if (rrt.obstacleFree(xNearest->point, xNew)) {
            Tree::Node *newNode = rrt.insertEdge(xNearest, xNew);
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
    std::cout.precision(8);
    std::cout << std::fixed;
    freopen("output.txt", "w", stdout);
    if (!finishNode) {
        std::cout << "Path not found\n";
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
                std::cout << "->" << res[i]->point;
            }
        }
    }
    std::cout << "\nTree:\n";
    rrt.printTree();
}

void RRTAlgorithm::launchWithVirt(const Map& map, const Algorithm& algo)
{
    RRT rrt(map, algo);
    Geometry::Point start = rrt.getStart();
    Geometry::Point finish = rrt.getFinish();
    Tree::Node *finishNode = nullptr;
    const double EPS = CI_STEP_SIZE * CI_STEP_SIZE;
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
            sf::CircleShape startCircle(0.1);
            startCircle.setPosition(sf::Vector2f(start.x, start.y));
            startCircle.setOrigin(startCircle.getRadius(), startCircle.getRadius());
            startCircle.setFillColor(sf::Color(170, 0, 0));
            sf::CircleShape finishCircle(CI_STEP_SIZE); // width / 600 * 3
            finishCircle.setOrigin(finishCircle.getRadius(), finishCircle.getRadius());
            finishCircle.setPosition(sf::Vector2f(finish.x, finish.y));
            finishCircle.setFillColor(sf::Color::Transparent);
            finishCircle.setOutlineThickness(0.2f);
            finishCircle.setOutlineColor(sf::Color(170, 0, 0));
            window.draw(startCircle);
            window.draw(finishCircle);
            window.display();
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
                        if (window.isOpen()) {
                            sf::CircleShape nodeCircle(0.1);
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
                } else {
                    if (window.isOpen()) {
                        sf::CircleShape nodeCircle(0.1);
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
                }
            }
            std::cout.precision(8);
            std::cout << std::fixed;
            freopen("output.txt", "w", stdout);
            if (!finishNode) {
                std::cout << "Path not found\n";
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
                        sf::Vertex line[] = {
                            sf::Vertex(sf::Vector2f(tmp->point.x, tmp->point.y)),
                            sf::Vertex(sf::Vector2f(res.back()->point.x, res.back()->point.y)),
                        };
                        line[0].color = sf::Color::Red;
                        line[1].color = sf::Color::Red;
                        window.draw(line, 2, sf::Lines);
                    }
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
            window.draw(startCircle);
            window.draw(finishCircle);
            window.display();
            std::cout << "\nTree:\n";
            rrt.printTree();
            isReady = true;
        }
    }
}

void RRTAlgorithm::launchWithVirtAfter(const Map& map, const Algorithm& algo)
{
    RRT rrt(map, algo);
    Geometry::Point finish = rrt.getFinish();
    Geometry::Point start = rrt.getStart();
    Tree::Node *finishNode = nullptr;
    const double EPS = CI_STEP_SIZE;
    const size_t numberOfIter = rrt.getNumberOfIterations();
    for (size_t it = 0; it < numberOfIter; ++it) {
        Geometry::Point xRand = rrt.getRandomPoint();
        Tree::Node *xNearest = rrt.getNearest(xRand);
        Geometry::Point xNew = rrt.steer(xNearest->point, xRand);
        if (rrt.obstacleFree(xNearest->point, xNew)) {
            Tree::Node *newNode = rrt.insertEdge(xNearest, xNew);
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
    size_t height = map.getMapHeight();
    size_t width = map.getMapWidth();
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(desktop.width - 100, desktop.height - 100, desktop.bitsPerPixel), "Algorithm", sf::Style::Close | sf::Style::Titlebar, settings);
    sf::View view(sf::FloatRect(0., 0., (float)width + 1, (float)height + 1));
    window.setView(view);
    // window.setFramerateLimit(60);
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
            sf::CircleShape startCircle(0.1);
            startCircle.setPosition(sf::Vector2f(start.x, start.y));
            startCircle.setOrigin(startCircle.getRadius(), startCircle.getRadius());
            startCircle.setFillColor(sf::Color(170, 0, 0));
            sf::CircleShape finishCircle(CI_STEP_SIZE); // width / 600 * 3
            finishCircle.setOrigin(finishCircle.getRadius(), finishCircle.getRadius());
            finishCircle.setPosition(sf::Vector2f(finish.x, finish.y));
            finishCircle.setFillColor(sf::Color::Transparent);
            finishCircle.setOutlineThickness(0.2f);
            finishCircle.setOutlineColor(sf::Color(170, 0, 0));
            window.draw(startCircle);
            window.draw(finishCircle);
            rrt.drawTree(window);
            if (finishNode) {
                Tree::Node *tmp = finishNode;
                while (tmp) {
                    if (tmp->parent) {
                        sf::Vertex line[] = {
                            sf::Vertex(sf::Vector2f(tmp->point.x, tmp->point.y)),
                            sf::Vertex(sf::Vector2f(tmp->parent->point.x, tmp->parent->point.y)),
                        };
                        line[0].color = sf::Color::Red;
                        line[1].color = sf::Color::Red;
                        window.draw(line, 2, sf::Lines);
                    }
                    tmp = tmp->parent;
                }
            }
            window.draw(startCircle);
            window.draw(finishCircle);
            window.display();
        }
        isReady = true;
    }
    std::cout.precision(8);
    std::cout << std::fixed;
    freopen("output.txt", "w", stdout);
    if (!finishNode) {
        std::cout << "Path not found\n";
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
                std::cout << "->" << res[i]->point;
            }
        }
    }
    std::cout << "\nTree:\n";
    rrt.printTree();
}

void RRTAlgorithm::launchWithVirtAfterWithoutTree(const Map& map, const Algorithm& algo)
{
    RRT rrt(map, algo);
    Geometry::Point finish = rrt.getFinish();
    Geometry::Point start = rrt.getStart();
    Tree::Node *finishNode = nullptr;
    const double EPS = CI_STEP_SIZE;
    const size_t numberOfIter = rrt.getNumberOfIterations();
    for (size_t it = 0; it < numberOfIter; ++it) {
        Geometry::Point xRand = rrt.getRandomPoint();
        Tree::Node *xNearest = rrt.getNearest(xRand);
        Geometry::Point xNew = rrt.steer(xNearest->point, xRand);
        if (rrt.obstacleFree(xNearest->point, xNew)) {
            Tree::Node *newNode = rrt.insertEdge(xNearest, xNew);
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
    size_t height = map.getMapHeight();
    size_t width = map.getMapWidth();
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(desktop.width - 100, desktop.height - 100, desktop.bitsPerPixel), "Algorithm", sf::Style::Close | sf::Style::Titlebar, settings);
    sf::View view(sf::FloatRect(0., 0., (float)width + 1, (float)height + 1));
    window.setView(view);
    // window.setFramerateLimit(60);
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
            sf::CircleShape startCircle(0.1);
            startCircle.setPosition(sf::Vector2f(start.x, start.y));
            startCircle.setOrigin(startCircle.getRadius(), startCircle.getRadius());
            startCircle.setFillColor(sf::Color(170, 0, 0));
            sf::CircleShape finishCircle(CI_STEP_SIZE); // width / 600 * 3
            finishCircle.setOrigin(finishCircle.getRadius(), finishCircle.getRadius());
            finishCircle.setPosition(sf::Vector2f(finish.x, finish.y));
            finishCircle.setFillColor(sf::Color::Transparent);
            finishCircle.setOutlineThickness(0.2f);
            finishCircle.setOutlineColor(sf::Color(170, 0, 0));
            window.draw(startCircle);
            window.draw(finishCircle);
            if (finishNode) {
                Tree::Node *tmp = finishNode;
                while (tmp) {
                    if (tmp->parent) {
                        sf::Vertex line[] = {
                            sf::Vertex(sf::Vector2f(tmp->point.x, tmp->point.y)),
                            sf::Vertex(sf::Vector2f(tmp->parent->point.x, tmp->parent->point.y)),
                        };
                        line[0].color = sf::Color::Red;
                        line[1].color = sf::Color::Red;
                        window.draw(line, 2, sf::Lines);
                    }
                    tmp = tmp->parent;
                }
            }
            window.display();
        }
        isReady = true;
    }
    std::cout.precision(8);
    std::cout << std::fixed;
    freopen("output.txt", "w", stdout);
    if (!finishNode) {
        std::cout << "Path not found\n";
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
                std::cout << "->" << res[i]->point;
            }
        }
    }
    std::cout << "\nTree:\n";
    rrt.printTree();
}