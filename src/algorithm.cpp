#include "algorithm.hpp"

bool Algorithm::buildAlgorithm(const char *filePath)
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
    pugi::xml_node algorithm = root.child(CS_TAG_ALGORITHM);
    if (!algorithm) {
        std::cerr << "Error! No '" << CS_TAG_ALGORITHM << "' tag found in XML file!" << '\n';
        return false;
    }
    bool hasST = false, hasNOI = false;
    for (pugi::xml_node tag: algorithm.children()) {
        std::string nameNode = tag.name();
        std::transform(nameNode.begin(), nameNode.end(), nameNode.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        if (nameNode == CS_TAG_SEARCHTYPE) {
            std::string algorithmType = tag.text().get();
            std::transform(algorithmType.begin(), algorithmType.end(), algorithmType.begin(), [](unsigned char c) {
                return std::tolower(c);
            });
            if (hasST) {
                std::cerr << "Warning! Repeat tag searchtype. The old value will be used!\n";
            } else {
                if (algorithmType == CS_TAG_RRT) {
                    searchType = CI_TAG_RRT;
                    hasST = true;
                } else if (algorithmType == CS_TAG_RRT_STAR) {
                    searchType = CI_TAG_RRT_STAR;
                    hasST = true;
                } else {
                    std::cerr << "Error!. Unknown algorithm " << algorithmType << " in " << CS_TAG_SEARCHTYPE << " tag.\n";
                    return false;
                }
            }
        } else if (nameNode == CS_TAG_NUMBER_OF_ITERATIONS) {
            if (hasNOI) {
                std::cerr << "Warning! Repeat tag numberofiterations. The old value will be used!\n";
            } else {
                hasNOI = true;
                numberOfIterations = tag.text().as_ullong();
                if (!numberOfIterations) {
                    std::cerr << "Error! Number of iterations should be positive integer number!\n";
                    return false;
                }
            }
        } else {
            std::cerr << "Warning! Unknown tag " << nameNode << ". It will be skipped!\n";
        }
    }
    if (!hasST) {
        std::cerr << "Error! No tag searchtype.\n";
        return false;
    }
    if (!hasNOI) {
        std::cerr << "Error! No tag numberofiterations.\n";
        return false;
    }
    return true;
}

Algorithm::Algorithm(const char *filePath)
{
    if (!buildAlgorithm(filePath)) {
        std::cerr << "Error! Input Error!\n";
        exit(EXIT_FAILURE);
    }
}