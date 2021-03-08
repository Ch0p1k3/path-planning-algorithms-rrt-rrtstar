#include "map.hpp"

bool Map::buildMap(const char *filePath) {
    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file(filePath);
    if (!result) {
        std::cerr << "XML [" << filePath << "] parsed with errors\nError description: " << result.description() << '\n';
        std::cerr << "Error offset: " << result.offset << "\n";
        return false;
    }
    pugi::xml_node root = document.child("root");
    if (!root) {
        std::cout << "Error! No '" << CS_TAG_ROOT << "' tag found in XML file!" << std::endl;
        return false;
    }
    
}

ssize_t Map::getMapHeight() const {
    return height;
}

ssize_t Map::getMapWidth() const {
    return width;
}

Geometry::Point Map::getStart() const {
    return start;
}

Geometry::Point Map::getFinish() const {
    return finish;
}