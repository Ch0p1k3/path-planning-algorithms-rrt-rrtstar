#include "outdata.hpp"

void OutData::fill(const std::string& filePath, const SearchResult& result)
{
    pugi::xml_document document;
    document.append_child(CS_TAG_ROOT);
    pugi::xml_node root = document.child(CS_TAG_ROOT);
    root.append_child(CS_TAG_TIME);
    std::string tmp = std::to_string(result.time);
    root.last_child().append_child(pugi::node_pcdata).set_value(tmp.c_str());
    root.append_child(CS_TAG_COUNT_OF_EDGES);
    tmp = std::to_string(result.countOfEdges);
    root.last_child().append_child(pugi::node_pcdata).set_value(tmp.c_str());
    root.append_child(CS_TAG_PATH_FOUND);
    if (result.pathFound) {
        root.last_child().append_child(pugi::node_pcdata).set_value("true");
        root.append_child(CS_TAG_RESULT_DISTANCE);
        tmp = std::to_string(result.distance);
        root.last_child().append_child(pugi::node_pcdata).set_value(tmp.c_str());
        root.append_child(CS_TAG_RESULT_PATH);
        size_t index = 0;
        for (const auto& e: result.path) {
            root.last_child().append_child(CS_TAG_NODE);
            pugi::xml_node node = root.last_child().last_child();
            node.append_attribute(CS_ATR_NUMBER) = index++;
            node.append_attribute(CS_ATR_X) = e.x;
            node.append_attribute(CS_ATR_Y) = e.y;
        }
    } else {
        root.last_child().set_value("false");
    }
    // document.print(std::cout);
    std::cout << "Saving result: " << document.save_file(filePath.c_str()) << '\n';
}