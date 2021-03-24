#include "map.hpp"

bool Map::buildMap(const char *filePath) 
{
    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file(filePath);
    if (!result) {
        std::cerr << "XML [" << filePath << "] parsed with errors\nError description: " << result.description() << '\n';
        std::cerr << "Error offset: " << result.offset << "\n";
        return false;
    }
    pugi::xml_node root = document.child(CS_TAG_ROOT);
    if (!root) {
        std::cerr << "Error! No '" << CS_TAG_ROOT << "' tag found in XML file!" << '\n';
        return false;
    }
    pugi::xml_node map = root.child(CS_TAG_MAP);
    if (!map) {
        std::cerr << "Error! No '" << CS_TAG_MAP << "' tag found in XML file!" << '\n';
        return false;
    }
    bool hasHeight = false, hasWidth = false, hasSTX = false, hasSTY = false, hasFINX = false, hasFINY = false, hasGrid = false;
    double startX = 0, startY = 0, finishX = 0, finishY = 0;
    pugi::xml_node gridNode;
    for (pugi::xml_node_iterator it = map.begin(); it != map.end(); ++it) {
        std::string nameNode = it->name();
        std::transform(nameNode.begin(), nameNode.end(), nameNode.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        if (nameNode == CS_TAG_HEIGHT) {
            if (hasHeight) {
                std::cerr << "Warning! Repeat tag height. The old value will be used!\n";
            } else {
                hasHeight = true;
                height = it->text().as_ullong();
                if (!height) {
                    std::cerr << "Error! Height should be positive integer number!\n";
                    return false;
                }
            }
        } else if (nameNode == CS_TAG_WIDTH) {
            if (hasWidth) {
                std::cerr << "Warning! Repeat tag width. The old value will be used!\n";
            } else {
                hasWidth = true;
                width = it->text().as_ullong();
                if (!width) {
                    std::cerr << "Error! Width should be positive integer number!\n";
                    return false;
                }
            }
        } else if (nameNode == CS_TAG_STARTX) {
            if (hasSTX) {
                std::cerr << "Warning! Repeat tag startx. The old value will be used!\n";
            } else {
                hasSTX = true;
                startX = it->text().as_double(std::numeric_limits<double>::infinity());
                if (std::isinf(startX)) {
                    std::cerr << "Error! Startx should be real number!\n";
                    return false;
                }
            }
        } else if (nameNode == CS_TAG_STARTY) {
            if (hasSTY) {
                std::cerr << "Warning! Repeat tag starty. The old value will be used!\n";
            } else {
                hasSTY = true;
                startY = it->text().as_double(std::numeric_limits<double>::infinity());
                if (std::isinf(startY)) {
                    std::cerr << "Error! Starty should be real number!\n";
                    return false;
                }
            }
        } else if (nameNode == CS_TAG_FINISHX) {
            if (hasFINX) {
                std::cerr << "Warning! Repeat tag finishx. The old value will be used!\n";
            } else {
                hasFINX = true;
                finishX = it->text().as_double(std::numeric_limits<double>::infinity());
                if (std::isinf(finishX)) {
                    std::cerr << "Error! Finishx should be real number!\n";
                    return false;
                }
            }
        } else if (nameNode == CS_TAG_FINISHY) {
            if (hasFINY) {
                std::cerr << "Warning! Repeat tag finishy. The old value will be used!\n";
            } else {
                hasFINY = true;
                finishY = it->text().as_double(std::numeric_limits<double>::infinity());
                if (std::isinf(finishY)) {
                    std::cerr << "Error! Finishy should be real number!\n";
                    return false;
                }
            }
        } else if (nameNode == CS_TAG_GRID) {
            if (hasGrid) {
                std::cerr << "Warning! Repeat tag grid. The old grid will be used!\n";
            } else {
                hasGrid = true;
                gridNode = *it;
            }
        } else {
            std::cerr << "Warning! Unknown tag " << nameNode << ". It will be skipped!\n";
        }
    }
    if (!hasHeight) {
        std::cerr << "Error! No tag height.\n";
        return false;
    }
    if (!hasWidth) {
        std::cerr << "Error! No tag width.\n";
        return false;
    }
    if (!hasSTX) {
        std::cerr << "Error! No tag startx.\n";
        return false;
    }
    if (!hasSTY) {
        std::cerr << "Error! No tag starty.\n";
        return false;
    }
    if (!hasFINX) {
        std::cerr << "Error! No tag finishx.\n";
        return false;
    }
    if (!hasFINY) {
        std::cerr << "Error! No tag finishy.\n";
        return false;
    }
    if (!hasGrid) {
        std::cerr << "Error! No tag grid.\n";
        return false;
    }
    start = {startX, startY};
    finish = {finishX, finishY};
    size_t i = 0;
    bool value = false;
    std::string rowStr;
    grid.resize(height);
    for (pugi::xml_node row: gridNode.children()) {
        if (i == height) {
            std::cerr << "Height of grid is greater than value of height.\n";
            return false;
        }
        rowStr = row.text().get();
        std::istringstream ss(rowStr);
        while (ss >> value) {
            grid[i].push_back(value);
        }
        if (grid[i].size() > width) {
            std::cerr << "Error! Width " << grid[i].size() << " in " << i << " line.\n";
            return false;
        }
        ++i;
    }
    return true;
}

Map::Map(const char *filePath) 
{
    if (!buildMap(filePath)) {
        std::cerr << "Error! Input Error!\n";
        exit(EXIT_FAILURE);
    }
}

const std::vector<bool>& Map::operator[](const size_t i) const
{
    return grid[i];
}

