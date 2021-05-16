#include "process.hpp"

const SearchResult RRTAlgorithm::launch(const Map& map, const Algorithm& algo)
{
    size_t countOfEdges = 0;
    RRT rrt(map, algo);
    Geometry::Point finish = rrt.getFinish();
    Tree::Node* finishNode = nullptr;
    const double EPS = rrt.getEps() * rrt.getEps();
    const size_t numberOfIter = rrt.getNumberOfIterations();
    auto time = std::chrono::steady_clock::now();
    for (size_t it = 0; it < numberOfIter; ++it) {
        Geometry::Point xRand = rrt.getRandomPoint();
        Tree::Node* xNearest = rrt.getNearest(xRand);
        Geometry::Point xNew = rrt.steer(xNearest->point, xRand);
        if (Geometry::euclideanMetric(xNearest->point, xNew) < 0.01) continue;
        if (rrt.obstacleFree(xNearest->point, xNew)) {
            Tree::Node* newNode = rrt.insertVertexAndEdge(xNearest, xNew);
            if (newNode) {
                ++countOfEdges;
                if (Geometry::euclideanMetric(newNode->point, finish) <= EPS) {
                    if (!finishNode) {
                        finishNode = newNode;
                    } else {
                        if (finishNode->getDistance() > newNode->getDistance()) {
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
    SearchResult searchResult;
    searchResult.time = timeRes;
    searchResult.countOfEdges = countOfEdges;
    std::cout << "Time: " << timeRes << "\nCount of edges: " << countOfEdges << '\n';
    if (!finishNode) {
        searchResult.pathFound = false;
        std::cout << "Path not found.\n";
    } else {
        searchResult.pathFound = true;
        searchResult.distance = finishNode->getDistance();
        std::cout << "Result distance: " << finishNode->getDistance() << '\n';
        Tree::Node* tmp = finishNode;
        std::vector<Geometry::Point> res;
        while (tmp) {
            res.push_back(tmp->point);
            tmp = tmp->parent;
        }
        std::cout << "Path:\n";
        std::reverse(res.begin(), res.end());
        searchResult.path = res;
        for (size_t i = 0; i < res.size(); ++i) {
            if (!i) {
                std::cout << res[i];
            } else {
                std::cout << "\n" << res[i];
            }
        }
        if (!res.empty()) std::cout << '\n';
    }
    return searchResult;
}

const SearchResult RRTAlgorithm::launchWithVis(const Map& map, const Algorithm& algo)
{
    SearchResult searchResult;
    size_t countOfEdges = 0;
    RRT rrt(map, algo);
    Geometry::Point start = rrt.getStart();
    Geometry::Point finish = rrt.getFinish();
    Tree::Node* finishNode = nullptr;
    const double EPS = rrt.getEps() * rrt.getEps();
    const size_t numberOfIter = rrt.getNumberOfIterations();
    size_t height = map.getMapHeight();
    size_t width = map.getMapWidth();
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    double coef = std::min((double)desktop.width / width, (double)desktop.height / height);
    sf::RenderWindow window(sf::VideoMode((unsigned int)(width * coef * 0.95), (unsigned int)(height * coef * 0.95), desktop.bitsPerPixel), "Algorithm", sf::Style::Close | sf::Style::Titlebar, settings);
    sf::View view(sf::FloatRect(0., 0., (float)width, (float)height));
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
            sf::CircleShape startCircle(((double)std::max(height, width) / 100) * 1);
            startCircle.setOrigin(startCircle.getRadius(), startCircle.getRadius());
            startCircle.setPosition(sf::Vector2f(start.x, start.y));
            startCircle.setFillColor(sf::Color(0, 106, 0));
            sf::CircleShape finishCircle(rrt.getEps());
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
                Tree::Node* xNearest = rrt.getNearest(xRand);
                Geometry::Point xNew = rrt.steer(xNearest->point, xRand);
                if (Geometry::euclideanMetric(xNearest->point, xNew) < 0.01) continue;
                if (rrt.obstacleFree(xNearest->point, xNew)) {
                    Tree::Node* newNode = rrt.insertVertexAndEdge(xNearest, xNew);
                    if (newNode) {
                        ++countOfEdges;
                        tmp = std::chrono::steady_clock::now();
                        if (window.isOpen()) {
                            sf::CircleShape nodeCircle(((double)std::max(height, width) / 100) * 0.1);
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
                                if (finishNode->getDistance() > newNode->getDistance()) {
                                    finishNode = newNode;
                                }
                            }
                            break;
                        }
                    }
                } else {
                    tmp = std::chrono::steady_clock::now();
                    if (window.isOpen()) {
                        sf::CircleShape nodeCircle(((double)std::max(height, width) / 100) * 0.1);
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
            searchResult.time = resTime;
            searchResult.countOfEdges = countOfEdges;
            std::cout << "Time: " << resTime << '\n';
            std::cout << "Count of edges: " << countOfEdges << '\n';
            if (!finishNode) {
                searchResult.pathFound = false;
                std::cout << "Path not found.\n";
            } else {
                searchResult.pathFound = true;
                searchResult.distance = finishNode->getDistance();
                std::cout << "Result distance: " << finishNode->getDistance() << '\n';
                Tree::Node* tmp = finishNode;
                std::vector<Geometry::Point> res;
                while (tmp) {
                    while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed) {
                            window.close();
                        }
                    }
                    if (window.isOpen() && !res.empty()) {
                        sf::CircleShape nodeCircle(((double)std::max(height, width) / 100) * 0.2);
                        nodeCircle.setFillColor(sf::Color(255, 169, 0));
                        nodeCircle.setOrigin(nodeCircle.getRadius(), nodeCircle.getRadius());
                        nodeCircle.setPosition(sf::Vector2f(tmp->point.x, tmp->point.y));
                        window.draw(nodeCircle);
                        nodeCircle.setPosition(sf::Vector2f(res.back().x, res.back().y));
                        window.draw(nodeCircle);
                        sf::RectangleShape line2(sf::Vector2f(std::sqrt(Geometry::euclideanMetric(res.back(), tmp->point)), (double)height / 100 * 0.2));
                        line2.rotate(std::atan2(res.back().y - tmp->point.y, res.back().x - tmp->point.x) / M_PI * 180);
                        line2.setOrigin(0, ((double)std::max(height, width) / 100 * 0.2) / 2);
                        line2.setPosition(tmp->point.x, tmp->point.y);
                        line2.setFillColor(sf::Color(255, 169, 0));
                        window.draw(line2);
                    }
                    res.push_back(tmp->point);
                    tmp = tmp->parent;
                }
                std::cout << "Path:\n";
                std::reverse(res.begin(), res.end());
                searchResult.path = res;
                for (size_t i = 0; i < res.size(); ++i) {
                    if (!i) {
                        std::cout << res[i];
                    } else {
                        std::cout << "\n" << res[i];
                    }
                }
                if (!res.empty()) std::cout << '\n';
            }
            window.draw(startCircle);
            window.draw(finishCircle);
            window.display();
            isReady = true;
        }
    }
    return searchResult;
}

const SearchResult RRTAlgorithm::launchWithVisAfter(const Map& map, const Algorithm& algo)
{
    SearchResult searchResult;
    size_t countOfEdges = 0;
    RRT rrt(map, algo);
    Geometry::Point finish = rrt.getFinish();
    Geometry::Point start = rrt.getStart();
    Tree::Node* finishNode = nullptr;
    const double EPS = rrt.getEps() * rrt.getEps();
    const size_t numberOfIter = rrt.getNumberOfIterations();
    auto time = std::chrono::steady_clock::now();
    for (size_t it = 0; it < numberOfIter; ++it) {
        Geometry::Point xRand = rrt.getRandomPoint();
        Tree::Node* xNearest = rrt.getNearest(xRand);
        Geometry::Point xNew = rrt.steer(xNearest->point, xRand);
        if (Geometry::euclideanMetric(xNearest->point, xNew) < 0.01) continue;
        if (rrt.obstacleFree(xNearest->point, xNew)) {
            Tree::Node* newNode = rrt.insertVertexAndEdge(xNearest, xNew);
            if (newNode) {
                ++countOfEdges;
                if (Geometry::euclideanMetric(newNode->point, finish) <= EPS) {
                    if (!finishNode) {
                        finishNode = newNode;
                    } else {
                        if (finishNode->getDistance() > newNode->getDistance()) {
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
    searchResult.time = resTime;
    searchResult.countOfEdges = countOfEdges;
    std::cout << "Time: " << resTime << '\n';
    std::cout << "Count of edges: " << countOfEdges << '\n';
    size_t height = map.getMapHeight();
    size_t width = map.getMapWidth();
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    double coef = std::min((double)desktop.width / width, (double)desktop.height / height);
    sf::RenderWindow window(sf::VideoMode((unsigned int)(width * coef * 0.95), (unsigned int)(height * coef * 0.95), desktop.bitsPerPixel), "Algorithm", sf::Style::Close | sf::Style::Titlebar, settings);
    sf::View view(sf::FloatRect(0., 0., (float)width, (float)height));
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
            sf::CircleShape startCircle(((double)std::max(height, width) / 100) * 1);
            startCircle.setOrigin(startCircle.getRadius(), startCircle.getRadius());
            startCircle.setPosition(sf::Vector2f(start.x, start.y));
            startCircle.setFillColor(sf::Color(0, 106, 0));
            sf::CircleShape finishCircle(rrt.getEps());
            finishCircle.setOrigin(finishCircle.getRadius(), finishCircle.getRadius());
            finishCircle.setPosition(sf::Vector2f(finish.x, finish.y));
            finishCircle.setFillColor(sf::Color::Transparent);
            finishCircle.setOutlineThickness(finishCircle.getRadius() / 100 * 40);
            finishCircle.setOutlineColor(sf::Color(255, 169, 0));
            rrt.drawTree(window);
            if (finishNode) {
                Tree::Node* tmp = finishNode;
                while (tmp) {
                    if (tmp->parent) {
                        sf::CircleShape nodeCircle(((double)std::max(height, width) / 100) * 0.2);
                        nodeCircle.setFillColor(sf::Color(255, 169, 0));
                        nodeCircle.setOrigin(nodeCircle.getRadius(), nodeCircle.getRadius());
                        nodeCircle.setPosition(sf::Vector2f(tmp->point.x, tmp->point.y));
                        window.draw(nodeCircle);
                        nodeCircle.setPosition(sf::Vector2f(tmp->parent->point.x, tmp->parent->point.y));
                        window.draw(nodeCircle);
                        sf::RectangleShape line2(sf::Vector2f(std::sqrt(Geometry::euclideanMetric(tmp->parent->point, tmp->point)), (double)height / 100 * 0.2));
                        line2.rotate(std::atan2(tmp->parent->point.y - tmp->point.y, tmp->parent->point.x - tmp->point.x) / M_PI * 180);
                        line2.setOrigin(0, ((double)std::max(height, width) / 100 * 0.2) / 2);
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
                searchResult.pathFound = false;
                std::cout << "Path not found.\n";
            } else {
                searchResult.pathFound = true;
                searchResult.distance = finishNode->getDistance();
                std::cout << "Result distance: " << finishNode->getDistance() << '\n';
                Tree::Node* tmp = finishNode;
                std::vector<Geometry::Point> res;
                while (tmp) {
                    res.push_back(tmp->point);
                    tmp = tmp->parent;
                }
                std::reverse(res.begin(), res.end());
                searchResult.path = res;
                for (size_t i = 0; i < res.size(); ++i) {
                    if (!i) {
                        std::cout << res[i];
                    } else {
                        std::cout << "\n" << res[i];
                    }
                }
                if (!res.empty()) std::cout << '\n';
            }
        }
        isReady = true;
    }
    return searchResult;
}

const SearchResult RRTAlgorithm::launchWithVisAfterWithoutTree(const Map& map, const Algorithm& algo)
{
    SearchResult searchResult;
    size_t countOfEdges = 0;
    RRT rrt(map, algo);
    Geometry::Point finish = rrt.getFinish();
    Geometry::Point start = rrt.getStart();
    Tree::Node* finishNode = nullptr;
    const double EPS = rrt.getEps() * rrt.getEps();
    const size_t numberOfIter = rrt.getNumberOfIterations();
    auto time = std::chrono::steady_clock::now();
    for (size_t it = 0; it < numberOfIter; ++it) {
        Geometry::Point xRand = rrt.getRandomPoint();
        Tree::Node* xNearest = rrt.getNearest(xRand);
        Geometry::Point xNew = rrt.steer(xNearest->point, xRand);
        if (Geometry::euclideanMetric(xNearest->point, xNew) < 0.01) continue;
        if (rrt.obstacleFree(xNearest->point, xNew)) {
            Tree::Node* newNode = rrt.insertVertexAndEdge(xNearest, xNew);
            if (newNode) {
                ++countOfEdges;
                if (Geometry::euclideanMetric(newNode->point, finish) <= EPS) {
                    if (!finishNode) {
                        finishNode = newNode;
                    } else {
                        if (finishNode->getDistance() > newNode->getDistance()) {
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
    searchResult.time = resTime;
    searchResult.countOfEdges = countOfEdges;
    std::cout << "Time: " << resTime << '\n';
    std::cout << "Count of edges: " << countOfEdges << '\n';
    size_t height = map.getMapHeight();
    size_t width = map.getMapWidth();
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    double coef = std::min((double)desktop.width / width, (double)desktop.height / height);
    sf::RenderWindow window(sf::VideoMode((unsigned int)(width * coef * 0.95), (unsigned int)(height * coef * 0.95), desktop.bitsPerPixel), "Algorithm", sf::Style::Close | sf::Style::Titlebar, settings);
    sf::View view(sf::FloatRect(0., 0., (float)width, (float)height));
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
            sf::CircleShape startCircle(((double)std::max(height, width) / 100) * 1);
            startCircle.setOrigin(startCircle.getRadius(), startCircle.getRadius());
            startCircle.setPosition(sf::Vector2f(start.x, start.y));
            startCircle.setFillColor(sf::Color(0, 106, 0));
            sf::CircleShape finishCircle(rrt.getEps());
            finishCircle.setOrigin(finishCircle.getRadius(), finishCircle.getRadius());
            finishCircle.setPosition(sf::Vector2f(finish.x, finish.y));
            finishCircle.setFillColor(sf::Color::Transparent);
            finishCircle.setOutlineThickness(finishCircle.getRadius() / 100 * 40);
            finishCircle.setOutlineColor(sf::Color(255, 169, 0));
            if (finishNode) {
                Tree::Node* tmp = finishNode;
                while (tmp) {
                    if (tmp->parent) {
                        sf::CircleShape nodeCircle(((double)std::max(height, width) / 100) * 0.2);
                        nodeCircle.setFillColor(sf::Color(255, 169, 0));
                        nodeCircle.setOrigin(nodeCircle.getRadius(), nodeCircle.getRadius());
                        nodeCircle.setPosition(sf::Vector2f(tmp->point.x, tmp->point.y));
                        window.draw(nodeCircle);
                        nodeCircle.setPosition(sf::Vector2f(tmp->parent->point.x, tmp->parent->point.y));
                        window.draw(nodeCircle);
                        sf::RectangleShape line2(sf::Vector2f(std::sqrt(Geometry::euclideanMetric(tmp->parent->point, tmp->point)), (double)height / 100 * 0.2));
                        line2.rotate(std::atan2(tmp->parent->point.y - tmp->point.y, tmp->parent->point.x - tmp->point.x) / M_PI * 180);
                        line2.setOrigin(0, ((double)std::max(height, width) / 100 * 0.2) / 2);
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
                searchResult.pathFound = false;
                std::cout << "Path not found.\n";
            } else {
                searchResult.pathFound = true;
                searchResult.distance = finishNode->getDistance();
                std::cout << "Result distance: " << finishNode->getDistance() << '\n';
                Tree::Node* tmp = finishNode;
                std::vector<Geometry::Point> res;
                while (tmp) {
                    res.push_back(tmp->point);
                    tmp = tmp->parent;
                }
                std::reverse(res.begin(), res.end());
                searchResult.path = res;
                for (size_t i = 0; i < res.size(); ++i) {
                    if (!i) {
                        std::cout << res[i];
                    } else {
                        std::cout << "\n" << res[i];
                    }
                }
                if (!res.empty()) std::cout << '\n';
            }
        }
        isReady = true;
    }
    return searchResult;
}

void Secret::launch(const Map& map, const Algorithm& algo)
{
    SearchResult searchResult;
    size_t countOfEdges = 0;
    RRT rrt(map, algo);
    Geometry::Point start = rrt.getStart();
    Geometry::Point finish = rrt.getFinish();
    const double EPS = rrt.getEps() * rrt.getEps();
    size_t height = map.getMapHeight();
    size_t width = map.getMapWidth();
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    double coef = std::min((double)desktop.width / width, (double)desktop.height / height);
    sf::RenderWindow window(sf::VideoMode((unsigned int)(width * coef * 0.95), (unsigned int)(height * coef * 0.95), desktop.bitsPerPixel), "Algorithm", sf::Style::Close | sf::Style::Titlebar, settings);
    sf::View view(sf::FloatRect(0., 0., (float)width, (float)height));
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
            sf::CircleShape startCircle(((double)std::max(height, width) / 100) * 1);
            startCircle.setOrigin(startCircle.getRadius(), startCircle.getRadius());
            startCircle.setPosition(sf::Vector2f(start.x, start.y));
            startCircle.setFillColor(sf::Color(0, 106, 0));
            sf::CircleShape finishCircle(rrt.getEps());
            finishCircle.setOrigin(finishCircle.getRadius(), finishCircle.getRadius());
            finishCircle.setPosition(sf::Vector2f(finish.x, finish.y));
            finishCircle.setFillColor(sf::Color::Transparent);
            finishCircle.setOutlineThickness(finishCircle.getRadius() / 100 * 40);
            finishCircle.setOutlineColor(sf::Color(255, 169, 0));
            window.draw(startCircle);
            window.draw(finishCircle);
            window.display();
            isReady = true;
        }
    }
}

const std::pair<SearchResult, SearchResult> RRTStarAlgorithm::launch(const Map& map, const Algorithm& algo)
{
    std::pair<SearchResult, SearchResult> searchResult;
    size_t countOfEdges = 0;
    RRTStar rrt(map, algo);
    Geometry::Point finish = rrt.getFinish();
    Tree::Node* finishNode = nullptr;
    std::vector<Tree::Node*> finishNodes;
    const double EPS = rrt.getEps() * rrt.getEps();
    const size_t numberOfIter = rrt.getNumberOfIterations();
    auto time = std::chrono::steady_clock::now();
    for (size_t it = 0; it < numberOfIter; ++it) {
        Geometry::Point xRand = rrt.getRandomPoint();
        Tree::Node* xNearest = rrt.getNearest(xRand);
        Geometry::Point xNew = rrt.steer(xNearest->point, xRand);
        if (Geometry::euclideanMetric(xNearest->point, xNew) < 0.01) continue;
        if (rrt.obstacleFree(xNearest->point, xNew)) {
            Tree::Node* xMin = xNearest;
            std::vector<Tree::Node*> areaNear;
            rrt.getNear(xNew, areaNear);
            for (const auto& x: areaNear) {
                if (Geometry::euclideanMetric(x->point, xNew) >= 0.01 && rrt.obstacleFree(x->point, xNew)) {
                    if (x->getDistance() + std::sqrt(Geometry::euclideanMetric(x->point, xNew)) < xMin->getDistance() + std::sqrt(Geometry::euclideanMetric(xMin->point, xNew))) {
                        xMin = x;
                    }
                }
            }  
            Tree::Node* newNode = rrt.insertVertexAndEdge(xMin, xNew);
            if (newNode) {
                ++countOfEdges;
                for (const auto& x: areaNear) {
                    if (x == xMin) {
                        continue;
                    }
                    if (Geometry::euclideanMetric(x->point, xNew) >= 0.01 && rrt.obstacleFree(x->point, xNew) && 
                        x->getDistance() > newNode->getDistance() + std::sqrt(Geometry::euclideanMetric(x->point, newNode->point))) {
                            auto xParent = x->parent;
                            rrt.changeEdge(xParent, x, newNode);
                        }
                }
                if (Geometry::euclideanMetric(newNode->point, finish) <= EPS) {
                    finishNodes.push_back(newNode);
                    if (finishNodes.size() == 1) {
                        searchResult.first.time = std::chrono::duration<double>(std::chrono::steady_clock::now() - time).count();
                        searchResult.first.countOfEdges = countOfEdges;
                        searchResult.first.pathFound = true;
                        searchResult.first.distance = finishNodes.back()->getDistance();
                        Tree::Node* tmp = finishNodes.back();
                        std::vector<Geometry::Point> res;
                        while (tmp) {
                            res.push_back(tmp->point);
                            tmp = tmp->parent;
                        }
                        std::reverse(res.begin(), res.end());
                        searchResult.first.path = res;
                    }
                }
            }
        }
    }
    for (const auto& node: finishNodes) {
        if (!finishNode) {
            finishNode = node;
        } else {
            if (finishNode->getDistance() > node->getDistance()) {
                finishNode = node;
            }
        }
    }
    double timeRes = std::chrono::duration<double>(std::chrono::steady_clock::now() - time).count();
    std::cout.precision(8);
    std::cout << std::fixed;
    searchResult.second.time = timeRes;
    searchResult.second.countOfEdges = countOfEdges;
    std::cout << "Time: " << timeRes << "\nCount of edges: " << countOfEdges << '\n';
    if (!finishNode) {
        searchResult.second.pathFound = false;
        std::cout << "Path not found.\n";
    } else {
        searchResult.second.pathFound = true;
        searchResult.second.distance = finishNode->getDistance();
        std::cout << "Result distance: " << finishNode->getDistance() << '\n';
        Tree::Node* tmp = finishNode;
        std::vector<Geometry::Point> res;
        while (tmp) {
            res.push_back(tmp->point);
            tmp = tmp->parent;
        }
        std::cout << "Path:\n";
        std::reverse(res.begin(), res.end());
        searchResult.second.path = res;
        for (size_t i = 0; i < res.size(); ++i) {
            if (!i) {
                std::cout << res[i];
            } else {
                std::cout << "\n" << res[i];
            }
        }
        if (!res.empty()) std::cout << '\n';
    }
    return searchResult;
}

const std::pair<SearchResult, SearchResult> RRTStarAlgorithm::launchWithVis(const Map& map, const Algorithm& algo)
{
    std::pair<SearchResult, SearchResult> searchResult;
    size_t countOfEdges = 0;
    RRTStar rrt(map, algo);
    Geometry::Point start = rrt.getStart();
    Geometry::Point finish = rrt.getFinish();
    Tree::Node* finishNode = nullptr;
    std::vector<Geometry::Point> prevFinishNode;
    std::vector<Tree::Node*> finishNodes;
    const double EPS = rrt.getEps() * rrt.getEps();
    const size_t numberOfIter = rrt.getNumberOfIterations();
    size_t height = map.getMapHeight();
    size_t width = map.getMapWidth();
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    double coef = std::min((double)desktop.width / width, (double)desktop.height / height);
    sf::RenderWindow window(sf::VideoMode((unsigned int)(width * coef * 0.95), (unsigned int)(height * coef * 0.95), desktop.bitsPerPixel), "Algorithm", sf::Style::Close | sf::Style::Titlebar, settings);
    sf::View view(sf::FloatRect(0., 0., (float)width, (float)height));
    window.setView(view);
    // window.setFramerateLimit(60);
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
            sf::CircleShape startCircle(((double)std::max(height, width) / 100) * 1);
            startCircle.setOrigin(startCircle.getRadius(), startCircle.getRadius());
            startCircle.setPosition(sf::Vector2f(start.x, start.y));
            startCircle.setFillColor(sf::Color(0, 106, 0));
            sf::CircleShape finishCircle(rrt.getEps());
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
                Tree::Node* xNearest = rrt.getNearest(xRand);
                Geometry::Point xNew = rrt.steer(xNearest->point, xRand);
                if (Geometry::euclideanMetric(xNearest->point, xNew) < 0.01) continue;
                if (rrt.obstacleFree(xNearest->point, xNew)) {
                    Tree::Node* xMin = xNearest;
                    std::vector<Tree::Node*> areaNear;
                    rrt.getNear(xNew, areaNear);
                    for (const auto& x: areaNear) {
                        if (Geometry::euclideanMetric(x->point, xNew) >= 0.01 && rrt.obstacleFree(x->point, xNew) ) {
                            if (x->getDistance() + std::sqrt(Geometry::euclideanMetric(x->point, xNew)) < xMin->getDistance() + std::sqrt(Geometry::euclideanMetric(xMin->point, xNew))) {
                                xMin = x;
                            }
                        }
                    }
                    Tree::Node* newNode = rrt.insertVertexAndEdge(xMin, xNew);
                    if (newNode) {
                        tmp = std::chrono::steady_clock::now();
                        if (window.isOpen()) {
                            sf::CircleShape nodeCircle(((double)std::max(height, width) / 100) * 0.1);
                            nodeCircle.setFillColor(sf::Color::Black);
                            nodeCircle.setOrigin(nodeCircle.getRadius(), nodeCircle.getRadius());
                            nodeCircle.setPosition(sf::Vector2f(xNew.x, xNew.y));
                            window.draw(nodeCircle);
                            sf::Vertex line[] = {
                                sf::Vertex(sf::Vector2f(xMin->point.x, xMin->point.y)),
                                sf::Vertex(sf::Vector2f(xNew.x, xNew.y)),
                            };
                            line[0].color = sf::Color::Black;
                            line[1].color = sf::Color::Black;
                            window.draw(line, 2, sf::Lines);
                            window.display();
                        }
                        resTime -= std::chrono::duration<double>(std::chrono::steady_clock::now() - tmp).count();
                        ++countOfEdges;
                        for (const auto& x: areaNear) {
                            if (x == xMin) {
                                continue;
                            }
                            if (Geometry::euclideanMetric(x->point, xNew) >= 0.01 && rrt.obstacleFree(x->point, xNew) && 
                                x->getDistance() > newNode->getDistance() + std::sqrt(Geometry::euclideanMetric(x->point, newNode->point))) {
                                    auto xParent = x->parent;
                                    rrt.changeEdge(xParent, x, newNode);
                                    tmp = std::chrono::steady_clock::now();
                                    if (window.isOpen()) {
                                        sf::Vertex line[] = {
                                            sf::Vertex(sf::Vector2f(xParent->point.x, xParent->point.y)),
                                            sf::Vertex(sf::Vector2f(x->point.x, x->point.y)),
                                        };
                                        line[0].color = sf::Color::White;
                                        line[1].color = sf::Color::White;
                                        window.draw(line, 2, sf::Lines);
                                        window.display();
                                        line[0] = sf::Vertex(sf::Vector2f(newNode->point.x, newNode->point.y));
                                        line[1] = sf::Vertex(sf::Vector2f(x->point.x, x->point.y));
                                        line[0].color = sf::Color::Black;
                                        line[1].color = sf::Color::Black;
                                        window.draw(line, 2, sf::Lines);
                                        window.draw(finishCircle);
                                        window.draw(startCircle);
                                    }
                                    resTime -= std::chrono::duration<double>(std::chrono::steady_clock::now() - tmp).count();
                            } else {
                                tmp = std::chrono::steady_clock::now();
                                if (window.isOpen()) {
                                    auto xParent = x->parent;
                                    sf::Vertex line[] = {
                                        sf::Vertex(sf::Vector2f(xParent->point.x, xParent->point.y)),
                                        sf::Vertex(sf::Vector2f(x->point.x, x->point.y)),
                                    };
                                    line[0].color = sf::Color::Black;
                                    line[1].color = sf::Color::Black;
                                    window.draw(line, 2, sf::Lines);
                                }
                                resTime -= std::chrono::duration<double>(std::chrono::steady_clock::now() - tmp).count();
                            }
                        }
                        tmp = std::chrono::steady_clock::now();
                        if (window.isOpen()) {
                            window.display();
                        }
                        resTime -= std::chrono::duration<double>(std::chrono::steady_clock::now() - tmp).count();
                        if (Geometry::euclideanMetric(newNode->point, finish) <= EPS) {
                            finishNodes.push_back(newNode);
                            if (finishNodes.size() == 1) {
                                searchResult.first.time = std::chrono::duration<double>(std::chrono::steady_clock::now() - time).count();
                                searchResult.first.countOfEdges = countOfEdges;
                                searchResult.first.pathFound = true;
                                searchResult.first.distance = finishNodes.back()->getDistance();
                                Tree::Node* tmp = finishNodes.back();
                                std::vector<Geometry::Point> res;
                                while (tmp) {
                                    res.push_back(tmp->point);
                                    tmp = tmp->parent;
                                }
                                std::reverse(res.begin(), res.end());
                                searchResult.first.path = res;
                            }
                        }
                        tmp = std::chrono::steady_clock::now();
                        for (const auto& node: finishNodes) {
                            if (!finishNode) {
                                finishNode = node;
                            } else {
                                if (finishNode->getDistance() > node->getDistance()) {
                                    finishNode = node;
                                }
                            }
                        }
                        Tree::Node* tmpNode = finishNode;
                        std::vector<Geometry::Point> res;
                        while (tmpNode) {
                            res.push_back(tmpNode->point);
                            tmpNode = tmpNode->parent;
                        }
                        if (res != prevFinishNode) {
                            if (prevFinishNode.size() == 1) {
                                while (window.pollEvent(event)) {
                                    if (event.type == sf::Event::Closed) {
                                        window.close();
                                    }
                                    sf::CircleShape nodeCircle(((double)std::max(height, width) / 100) * 0.2);
                                    nodeCircle.setFillColor(sf::Color::White);
                                    nodeCircle.setOrigin(nodeCircle.getRadius(), nodeCircle.getRadius());
                                    nodeCircle.setPosition(sf::Vector2f(prevFinishNode[0].x, prevFinishNode[0].y));
                                    window.draw(nodeCircle);
                                }
                            } else {
                                for (int i = prevFinishNode.size() - 2; i > -1; --i) {
                                    while (window.pollEvent(event)) {
                                        if (event.type == sf::Event::Closed) {
                                            window.close();
                                        }
                                    }
                                    if (window.isOpen()) {
                                        sf::CircleShape nodeCircle(((double)std::max(height, width) / 100) * 0.2);
                                        nodeCircle.setFillColor(sf::Color::White);
                                        nodeCircle.setOrigin(nodeCircle.getRadius(), nodeCircle.getRadius());
                                        nodeCircle.setPosition(sf::Vector2f(prevFinishNode[i].x, prevFinishNode[i].y));
                                        window.draw(nodeCircle);
                                        nodeCircle.setPosition(sf::Vector2f(prevFinishNode[i + 1].x, prevFinishNode[i + 1].y));
                                        window.draw(nodeCircle);
                                        sf::RectangleShape line2(sf::Vector2f(std::sqrt(Geometry::euclideanMetric(prevFinishNode[i], prevFinishNode[i + 1])), (double)height / 100 * 0.2));
                                        line2.rotate(std::atan2(prevFinishNode[i].y - prevFinishNode[i + 1].y, prevFinishNode[i].x - prevFinishNode[i + 1].x) / M_PI * 180);
                                        line2.setOrigin(0, ((double)std::max(height, width) / 100 * 0.2) / 2);
                                        line2.setPosition(prevFinishNode[i + 1].x, prevFinishNode[i + 1].y);
                                        line2.setFillColor(sf::Color::White);
                                        window.draw(line2);
                                    }
                                }
                            }
                            rrt.drawTree(window);
                            if (res.size() == 1) {
                                while (window.pollEvent(event)) {
                                    if (event.type == sf::Event::Closed) {
                                        window.close();
                                    }
                                    sf::CircleShape nodeCircle(((double)std::max(height, width) / 100) * 0.2);
                                    nodeCircle.setFillColor(sf::Color(255, 169, 0));
                                    nodeCircle.setOrigin(nodeCircle.getRadius(), nodeCircle.getRadius());
                                    nodeCircle.setPosition(sf::Vector2f(res[0].x, res[0].y));
                                    window.draw(nodeCircle);
                                }
                            } else {
                                for (int i = res.size() - 2; i > -1; --i) {
                                    while (window.pollEvent(event)) {
                                        if (event.type == sf::Event::Closed) {
                                            window.close();
                                        }
                                    }
                                    if (window.isOpen()) {
                                        sf::CircleShape nodeCircle(((double)std::max(height, width) / 100) * 0.2);
                                        nodeCircle.setFillColor(sf::Color(255, 169, 0));
                                        nodeCircle.setOrigin(nodeCircle.getRadius(), nodeCircle.getRadius());
                                        nodeCircle.setPosition(sf::Vector2f(res[i].x, res[i].y));
                                        window.draw(nodeCircle);
                                        nodeCircle.setPosition(sf::Vector2f(res[i + 1].x, res[i + 1].y));
                                        window.draw(nodeCircle);
                                        sf::RectangleShape line2(sf::Vector2f(std::sqrt(Geometry::euclideanMetric(res[i], res[i + 1])), (double)height / 100 * 0.2));
                                        line2.rotate(std::atan2(res[i].y - res[i + 1].y, res[i].x - res[i + 1].x) / M_PI * 180);
                                        line2.setOrigin(0, ((double)std::max(height, width) / 100 * 0.2) / 2);
                                        line2.setPosition(res[i + 1].x, res[i + 1].y);
                                        line2.setFillColor(sf::Color(255, 169, 0));
                                        window.draw(line2);
                                    }
                                }
                            }
                            prevFinishNode = res;
                            window.display();
                        } else {
                            // rrt.drawTree(window);
                            if (res.size() == 1) {
                                while (window.pollEvent(event)) {
                                    if (event.type == sf::Event::Closed) {
                                        window.close();
                                    }
                                    sf::CircleShape nodeCircle(((double)std::max(height, width) / 100) * 0.2);
                                    nodeCircle.setFillColor(sf::Color(255, 169, 0));
                                    nodeCircle.setOrigin(nodeCircle.getRadius(), nodeCircle.getRadius());
                                    nodeCircle.setPosition(sf::Vector2f(res[0].x, res[0].y));
                                    window.draw(nodeCircle);
                                }
                            } else {
                                for (int i = res.size() - 2; i > -1; --i) {
                                    while (window.pollEvent(event)) {
                                        if (event.type == sf::Event::Closed) {
                                            window.close();
                                        }
                                    }
                                    if (window.isOpen()) {
                                        sf::CircleShape nodeCircle(((double)std::max(height, width) / 100) * 0.2);
                                        nodeCircle.setFillColor(sf::Color(255, 169, 0));
                                        nodeCircle.setOrigin(nodeCircle.getRadius(), nodeCircle.getRadius());
                                        nodeCircle.setPosition(sf::Vector2f(res[i].x, res[i].y));
                                        window.draw(nodeCircle);
                                        nodeCircle.setPosition(sf::Vector2f(res[i + 1].x, res[i + 1].y));
                                        window.draw(nodeCircle);
                                        sf::RectangleShape line2(sf::Vector2f(std::sqrt(Geometry::euclideanMetric(res[i], res[i + 1])), (double)height / 100 * 0.2));
                                        line2.rotate(std::atan2(res[i].y - res[i + 1].y, res[i].x - res[i + 1].x) / M_PI * 180);
                                        line2.setOrigin(0, ((double)std::max(height, width) / 100 * 0.2) / 2);
                                        line2.setPosition(res[i + 1].x, res[i + 1].y);
                                        line2.setFillColor(sf::Color(255, 169, 0));
                                        window.draw(line2);
                                    }
                                }
                            }
                            window.display();
                        }
                        resTime -= std::chrono::duration<double>(std::chrono::steady_clock::now() - tmp).count();
                    }
                }
            }
            for (const auto& node: finishNodes) {
                if (!finishNode) {
                    finishNode = node;
                } else {
                    if (finishNode->getDistance() > node->getDistance()) {
                        finishNode = node;
                    }
                }
            }
            resTime += std::chrono::duration<double>(std::chrono::steady_clock::now() - time).count();
            std::cout.precision(8);
            std::cout << std::fixed;
            searchResult.second.time = resTime;
            searchResult.second.countOfEdges = countOfEdges;
            std::cout << "Time: " << resTime << '\n';
            std::cout << "Count of edges: " << countOfEdges << '\n';
            if (!finishNode) {
                searchResult.second.pathFound = false;
                std::cout << "Path not found.\n";
            } else {
                searchResult.second.pathFound = true;
                searchResult.second.distance = finishNode->getDistance();
                std::cout << "Result distance: " << finishNode->getDistance() << '\n';
                Tree::Node* tmpNode = finishNode;
                std::vector<Geometry::Point> res;
                while (tmpNode) {
                    res.push_back(tmpNode->point);
                    tmpNode = tmpNode->parent;
                }
                if (res != prevFinishNode) {
                    if (prevFinishNode.size() == 1) {
                        while (window.pollEvent(event)) {
                            if (event.type == sf::Event::Closed) {
                                window.close();
                            }
                            sf::CircleShape nodeCircle(((double)std::max(height, width) / 100) * 0.2);
                            nodeCircle.setFillColor(sf::Color::White);
                            nodeCircle.setOrigin(nodeCircle.getRadius(), nodeCircle.getRadius());
                            nodeCircle.setPosition(sf::Vector2f(prevFinishNode[0].x, prevFinishNode[0].y));
                            window.draw(nodeCircle);
                        }
                    } else {
                        for (int i = prevFinishNode.size() - 2; i > -1; --i) {
                            while (window.pollEvent(event)) {
                                if (event.type == sf::Event::Closed) {
                                    window.close();
                                }
                            }
                            if (window.isOpen()) {
                                sf::CircleShape nodeCircle(((double)std::max(height, width) / 100) * 0.2);
                                nodeCircle.setFillColor(sf::Color::White);
                                nodeCircle.setOrigin(nodeCircle.getRadius(), nodeCircle.getRadius());
                                nodeCircle.setPosition(sf::Vector2f(prevFinishNode[i].x, prevFinishNode[i].y));
                                window.draw(nodeCircle);
                                nodeCircle.setPosition(sf::Vector2f(prevFinishNode[i + 1].x, prevFinishNode[i + 1].y));
                                window.draw(nodeCircle);
                                sf::RectangleShape line2(sf::Vector2f(std::sqrt(Geometry::euclideanMetric(prevFinishNode[i], prevFinishNode[i + 1])), (double)height / 100 * 0.2));
                                line2.rotate(std::atan2(prevFinishNode[i].y - prevFinishNode[i + 1].y, prevFinishNode[i].x - prevFinishNode[i + 1].x) / M_PI * 180);
                                line2.setOrigin(0, ((double)std::max(height, width) / 100 * 0.2) / 2);
                                line2.setPosition(prevFinishNode[i + 1].x, prevFinishNode[i + 1].y);
                                line2.setFillColor(sf::Color::White);
                                window.draw(line2);
                            }
                        }
                    }
                    rrt.drawTree(window);
                    if (res.size() == 1) {
                        while (window.pollEvent(event)) {
                            if (event.type == sf::Event::Closed) {
                                window.close();
                            }
                            sf::CircleShape nodeCircle(((double)std::max(height, width) / 100) * 0.2);
                            nodeCircle.setFillColor(sf::Color(255, 169, 0));
                            nodeCircle.setOrigin(nodeCircle.getRadius(), nodeCircle.getRadius());
                            nodeCircle.setPosition(sf::Vector2f(res[0].x, res[0].y));
                            window.draw(nodeCircle);
                        }
                    } else {
                        for (int i = res.size() - 2; i > -1; --i) {
                            while (window.pollEvent(event)) {
                                if (event.type == sf::Event::Closed) {
                                    window.close();
                                }
                            }
                            if (window.isOpen()) {
                                sf::CircleShape nodeCircle(((double)std::max(height, width) / 100) * 0.2);
                                nodeCircle.setFillColor(sf::Color(255, 169, 0));
                                nodeCircle.setOrigin(nodeCircle.getRadius(), nodeCircle.getRadius());
                                nodeCircle.setPosition(sf::Vector2f(res[i].x, res[i].y));
                                window.draw(nodeCircle);
                                nodeCircle.setPosition(sf::Vector2f(res[i + 1].x, res[i + 1].y));
                                window.draw(nodeCircle);
                                sf::RectangleShape line2(sf::Vector2f(std::sqrt(Geometry::euclideanMetric(res[i], res[i + 1])), (double)height / 100 * 0.2));
                                line2.rotate(std::atan2(res[i].y - res[i + 1].y, res[i].x - res[i + 1].x) / M_PI * 180);
                                line2.setOrigin(0, ((double)std::max(height, width) / 100 * 0.2) / 2);
                                line2.setPosition(res[i + 1].x, res[i + 1].y);
                                line2.setFillColor(sf::Color(255, 169, 0));
                                window.draw(line2);
                            }
                        }
                    }
                    window.display();
                } else {
                    rrt.drawTree(window);
                    if (res.size() == 1) {
                        while (window.pollEvent(event)) {
                            if (event.type == sf::Event::Closed) {
                                window.close();
                            }
                            sf::CircleShape nodeCircle(((double)std::max(height, width) / 100) * 0.2);
                            nodeCircle.setFillColor(sf::Color(255, 169, 0));
                            nodeCircle.setOrigin(nodeCircle.getRadius(), nodeCircle.getRadius());
                            nodeCircle.setPosition(sf::Vector2f(res[0].x, res[0].y));
                            window.draw(nodeCircle);
                        }
                    } else {
                        for (int i = res.size() - 2; i > -1; --i) {
                            while (window.pollEvent(event)) {
                                if (event.type == sf::Event::Closed) {
                                    window.close();
                                }
                            }
                            if (window.isOpen()) {
                                sf::CircleShape nodeCircle(((double)std::max(height, width) / 100) * 0.2);
                                nodeCircle.setFillColor(sf::Color(255, 169, 0));
                                nodeCircle.setOrigin(nodeCircle.getRadius(), nodeCircle.getRadius());
                                nodeCircle.setPosition(sf::Vector2f(res[i].x, res[i].y));
                                window.draw(nodeCircle);
                                nodeCircle.setPosition(sf::Vector2f(res[i + 1].x, res[i + 1].y));
                                window.draw(nodeCircle);
                                sf::RectangleShape line2(sf::Vector2f(std::sqrt(Geometry::euclideanMetric(res[i], res[i + 1])), (double)height / 100 * 0.2));
                                line2.rotate(std::atan2(res[i].y - res[i + 1].y, res[i].x - res[i + 1].x) / M_PI * 180);
                                line2.setOrigin(0, ((double)std::max(height, width) / 100 * 0.2) / 2);
                                line2.setPosition(res[i + 1].x, res[i + 1].y);
                                line2.setFillColor(sf::Color(255, 169, 0));
                                window.draw(line2);
                            }
                        }
                    }
                    window.display();
                }
                std::cout << "Path:\n";
                std::reverse(res.begin(), res.end());
                searchResult.second.path = res;
                for (size_t i = 0; i < res.size(); ++i) {
                    if (!i) {
                        std::cout << res[i];
                    } else {
                        std::cout << "\n" << res[i];
                    }
                }
                if (!res.empty()) std::cout << '\n';
            }
            window.draw(startCircle);
            window.draw(finishCircle);
            window.display();
            isReady = true;
        }
    }
    return searchResult;
}


const std::pair<SearchResult, SearchResult> RRTStarAlgorithm::launchWithVisAfter(const Map& map, const Algorithm& algo)
{
    std::pair<SearchResult, SearchResult> searchResult;
    size_t countOfEdges = 0;
    RRTStar rrt(map, algo);
    Geometry::Point finish = rrt.getFinish();
    Geometry::Point start = rrt.getStart();
    Tree::Node* finishNode = nullptr;
    std::vector<Tree::Node*> finishNodes;
    const double EPS = rrt.getEps() * rrt.getEps();
    const size_t numberOfIter = rrt.getNumberOfIterations();
    auto time = std::chrono::steady_clock::now();
    for (size_t it = 0; it < numberOfIter; ++it) {
        Geometry::Point xRand = rrt.getRandomPoint();
        Tree::Node* xNearest = rrt.getNearest(xRand);
        Geometry::Point xNew = rrt.steer(xNearest->point, xRand);
        if (Geometry::euclideanMetric(xNearest->point, xNew) < 0.01) continue;
        if (rrt.obstacleFree(xNearest->point, xNew)) {
            Tree::Node* xMin = xNearest;
            std::vector<Tree::Node*> areaNear;
            rrt.getNear(xNew, areaNear);
            for (const auto& x: areaNear) {
                if (Geometry::euclideanMetric(x->point, xNew) >= 0.01 && rrt.obstacleFree(x->point, xNew)) {
                    if (x->getDistance() + std::sqrt(Geometry::euclideanMetric(x->point, xNew)) < xMin->getDistance() + std::sqrt(Geometry::euclideanMetric(xMin->point, xNew))) {
                        xMin = x;
                    }
                }
            }  
            Tree::Node* newNode = rrt.insertVertexAndEdge(xMin, xNew);
            if (newNode) {
                ++countOfEdges;
                for (const auto& x: areaNear) {
                    if (x == xMin) {
                        continue;
                    }
                    if (Geometry::euclideanMetric(x->point, xNew) >= 0.01 && rrt.obstacleFree(x->point, xNew) && 
                        x->getDistance() > newNode->getDistance() + std::sqrt(Geometry::euclideanMetric(x->point, newNode->point))) {
                            auto xParent = x->parent;
                            rrt.changeEdge(xParent, x, newNode);
                        }
                }
                if (Geometry::euclideanMetric(newNode->point, finish) <= EPS) {
                    finishNodes.push_back(newNode);
                    if (finishNodes.size() == 1) {
                        searchResult.first.time = std::chrono::duration<double>(std::chrono::steady_clock::now() - time).count();
                        searchResult.first.countOfEdges = countOfEdges;
                        searchResult.first.pathFound = true;
                        searchResult.first.distance = finishNodes.back()->getDistance();
                        Tree::Node* tmp = finishNodes.back();
                        std::vector<Geometry::Point> res;
                        while (tmp) {
                            res.push_back(tmp->point);
                            tmp = tmp->parent;
                        }
                        std::reverse(res.begin(), res.end());
                        searchResult.first.path = res;
                    }
                }
            }
        }
    }
    for (const auto& node: finishNodes) {
        if (!finishNode) {
            finishNode = node;
        } else {
            if (finishNode->getDistance() > node->getDistance()) {
                finishNode = node;
            }
        }
    }
    double resTime = std::chrono::duration<double>(std::chrono::steady_clock::now() - time).count();
    std::cout.precision(8);
    std::cout << std::fixed;
    searchResult.second.time = resTime;
    searchResult.second.countOfEdges = countOfEdges;
    std::cout << "Time: " << resTime << '\n';
    std::cout << "Count of edges: " << countOfEdges << '\n';
    size_t height = map.getMapHeight();
    size_t width = map.getMapWidth();
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    double coef = std::min((double)desktop.width / width, (double)desktop.height / height);
    sf::RenderWindow window(sf::VideoMode((unsigned int)(width * coef * 0.95), (unsigned int)(height * coef * 0.95), desktop.bitsPerPixel), "Algorithm", sf::Style::Close | sf::Style::Titlebar, settings);
    sf::View view(sf::FloatRect(0., 0., (float)width, (float)height));
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
            sf::CircleShape startCircle(((double)std::max(height, width) / 100) * 1);
            startCircle.setOrigin(startCircle.getRadius(), startCircle.getRadius());
            startCircle.setPosition(sf::Vector2f(start.x, start.y));
            startCircle.setFillColor(sf::Color(0, 106, 0));
            sf::CircleShape finishCircle(rrt.getEps());
            finishCircle.setOrigin(finishCircle.getRadius(), finishCircle.getRadius());
            finishCircle.setPosition(sf::Vector2f(finish.x, finish.y));
            finishCircle.setFillColor(sf::Color::Transparent);
            finishCircle.setOutlineThickness(finishCircle.getRadius() / 100 * 40);
            finishCircle.setOutlineColor(sf::Color(255, 169, 0));
            rrt.drawTree(window);
            if (finishNode) {
                Tree::Node* tmp = finishNode;
                while (tmp) {
                    if (tmp->parent) {
                        sf::CircleShape nodeCircle(((double)std::max(height, width) / 100) * 0.2);
                        nodeCircle.setFillColor(sf::Color(255, 169, 0));
                        nodeCircle.setOrigin(nodeCircle.getRadius(), nodeCircle.getRadius());
                        nodeCircle.setPosition(sf::Vector2f(tmp->point.x, tmp->point.y));
                        window.draw(nodeCircle);
                        nodeCircle.setPosition(sf::Vector2f(tmp->parent->point.x, tmp->parent->point.y));
                        window.draw(nodeCircle);
                        sf::RectangleShape line2(sf::Vector2f(std::sqrt(Geometry::euclideanMetric(tmp->parent->point, tmp->point)), (double)height / 100 * 0.2));
                        line2.rotate(std::atan2(tmp->parent->point.y - tmp->point.y, tmp->parent->point.x - tmp->point.x) / M_PI * 180);
                        line2.setOrigin(0, ((double)std::max(height, width) / 100 * 0.2) / 2);
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
                searchResult.second.pathFound = false;
                std::cout << "Path not found.\n";
            } else {
                searchResult.second.pathFound = true;
                searchResult.second.distance = finishNode->getDistance();
                std::cout << "Result distance: " << finishNode->getDistance() << '\n';
                Tree::Node* tmp = finishNode;
                std::vector<Geometry::Point> res;
                while (tmp) {
                    res.push_back(tmp->point);
                    tmp = tmp->parent;
                }
                std::reverse(res.begin(), res.end());
                searchResult.second.path = res;
                for (size_t i = 0; i < res.size(); ++i) {
                    if (!i) {
                        std::cout << res[i];
                    } else {
                        std::cout << "\n" << res[i];
                    }
                }
                if (!res.empty()) std::cout << '\n';
            }
        }
        isReady = true;
    }
    return searchResult;
}

const std::pair<SearchResult, SearchResult> RRTStarAlgorithm::launchWithVisAfterWithoutTree(const Map& map, const Algorithm& algo)
{
    std::pair<SearchResult, SearchResult> searchResult;
    size_t countOfEdges = 0;
    RRTStar rrt(map, algo);
    Geometry::Point finish = rrt.getFinish();
    Geometry::Point start = rrt.getStart();
    Tree::Node* finishNode = nullptr;
    std::vector<Tree::Node*> finishNodes;
    const double EPS = rrt.getEps() * rrt.getEps();
    const size_t numberOfIter = rrt.getNumberOfIterations();
    auto time = std::chrono::steady_clock::now();
    for (size_t it = 0; it < numberOfIter; ++it) {
        Geometry::Point xRand = rrt.getRandomPoint();
        Tree::Node* xNearest = rrt.getNearest(xRand);
        Geometry::Point xNew = rrt.steer(xNearest->point, xRand);
        if (Geometry::euclideanMetric(xNearest->point, xNew) < 0.01) continue;
        if (rrt.obstacleFree(xNearest->point, xNew)) {
            Tree::Node* xMin = xNearest;
            std::vector<Tree::Node*> areaNear;
            rrt.getNear(xNew, areaNear);
            for (const auto& x: areaNear) {
                if (Geometry::euclideanMetric(x->point, xNew) >= 0.01 && rrt.obstacleFree(x->point, xNew)) {
                    if (x->getDistance() + std::sqrt(Geometry::euclideanMetric(x->point, xNew)) < xMin->getDistance() + std::sqrt(Geometry::euclideanMetric(xMin->point, xNew))) {
                        xMin = x;
                    }
                }
            }  
            Tree::Node* newNode = rrt.insertVertexAndEdge(xMin, xNew);
            if (newNode) {
                ++countOfEdges;
                for (const auto& x: areaNear) {
                    if (x == xMin) {
                        continue;
                    }
                    if (Geometry::euclideanMetric(x->point, xNew) >= 0.01 && rrt.obstacleFree(x->point, xNew) && 
                        x->getDistance() > newNode->getDistance() + std::sqrt(Geometry::euclideanMetric(x->point, newNode->point))) {
                            auto xParent = x->parent;
                            rrt.changeEdge(xParent, x, newNode);
                        }
                }
                if (Geometry::euclideanMetric(newNode->point, finish) <= EPS) {
                    finishNodes.push_back(newNode);
                    if (finishNodes.size() == 1) {
                        searchResult.first.time = std::chrono::duration<double>(std::chrono::steady_clock::now() - time).count();
                        searchResult.first.countOfEdges = countOfEdges;
                        searchResult.first.pathFound = true;
                        searchResult.first.distance = finishNodes.back()->getDistance();
                        Tree::Node* tmp = finishNodes.back();
                        std::vector<Geometry::Point> res;
                        while (tmp) {
                            res.push_back(tmp->point);
                            tmp = tmp->parent;
                        }
                        std::reverse(res.begin(), res.end());
                        searchResult.first.path = res;
                    }
                }
            }
        }
    }
    for (const auto& node: finishNodes) {
        if (!finishNode) {
            finishNode = node;
        } else {
            if (finishNode->getDistance() > node->getDistance()) {
                finishNode = node;
            }
        }
    }
    double resTime = std::chrono::duration<double>(std::chrono::steady_clock::now() - time).count();
    std::cout.precision(8);
    std::cout << std::fixed;
    searchResult.second.time = resTime;
    searchResult.second.countOfEdges = countOfEdges;
    std::cout << "Time: " << resTime << '\n';
    std::cout << "Count of edges: " << countOfEdges << '\n';
    size_t height = map.getMapHeight();
    size_t width = map.getMapWidth();
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    double coef = std::min((double)desktop.width / width, (double)desktop.height / height);
    sf::RenderWindow window(sf::VideoMode((unsigned int)(width * coef * 0.95), (unsigned int)(height * coef * 0.95), desktop.bitsPerPixel), "Algorithm", sf::Style::Close | sf::Style::Titlebar, settings);
    sf::View view(sf::FloatRect(0., 0., (float)width, (float)height));
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
            sf::CircleShape startCircle(((double)std::max(height, width) / 100) * 1);
            startCircle.setOrigin(startCircle.getRadius(), startCircle.getRadius());
            startCircle.setPosition(sf::Vector2f(start.x, start.y));
            startCircle.setFillColor(sf::Color(0, 106, 0));
            sf::CircleShape finishCircle(rrt.getEps());
            finishCircle.setOrigin(finishCircle.getRadius(), finishCircle.getRadius());
            finishCircle.setPosition(sf::Vector2f(finish.x, finish.y));
            finishCircle.setFillColor(sf::Color::Transparent);
            finishCircle.setOutlineThickness(finishCircle.getRadius() / 100 * 40);
            finishCircle.setOutlineColor(sf::Color(255, 169, 0));
            // rrt.drawTree(window);
            if (finishNode) {
                Tree::Node* tmp = finishNode;
                while (tmp) {
                    if (tmp->parent) {
                        sf::CircleShape nodeCircle(((double)std::max(height, width) / 100) * 0.2);
                        nodeCircle.setFillColor(sf::Color(255, 169, 0));
                        nodeCircle.setOrigin(nodeCircle.getRadius(), nodeCircle.getRadius());
                        nodeCircle.setPosition(sf::Vector2f(tmp->point.x, tmp->point.y));
                        window.draw(nodeCircle);
                        nodeCircle.setPosition(sf::Vector2f(tmp->parent->point.x, tmp->parent->point.y));
                        window.draw(nodeCircle);
                        sf::RectangleShape line2(sf::Vector2f(std::sqrt(Geometry::euclideanMetric(tmp->parent->point, tmp->point)), (double)height / 100 * 0.2));
                        line2.rotate(std::atan2(tmp->parent->point.y - tmp->point.y, tmp->parent->point.x - tmp->point.x) / M_PI * 180);
                        line2.setOrigin(0, ((double)std::max(height, width) / 100 * 0.2) / 2);
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
                searchResult.second.pathFound = false;
                std::cout << "Path not found.\n";
            } else {
                searchResult.second.pathFound = true;
                searchResult.second.distance = finishNode->getDistance();
                std::cout << "Result distance: " << finishNode->getDistance() << '\n';
                Tree::Node* tmp = finishNode;
                std::vector<Geometry::Point> res;
                while (tmp) {
                    res.push_back(tmp->point);
                    tmp = tmp->parent;
                }
                std::reverse(res.begin(), res.end());
                searchResult.second.path = res;
                for (size_t i = 0; i < res.size(); ++i) {
                    if (!i) {
                        std::cout << res[i];
                    } else {
                        std::cout << "\n" << res[i];
                    }
                }
                if (!res.empty()) std::cout << '\n';
            }
        }
        isReady = true;
    }
    return searchResult;
}