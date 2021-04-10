#include "log.hpp"


short Log::buildLog(const char *filePath)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file(filePath);
    if (!result) {
        std::cerr << "XML [" << filePath << "] parsed with errors\nError description: " << result.description() << '\n';
        std::cerr << "Error offset: " << result.offset << "\n";
        return -1;
    }
    pugi::xml_node root = document.child(CS_TAG_ROOT);
    if (!root) {
        std::cerr << "Error! No '" << CS_TAG_ROOT << "' tag found in XML file!" << '\n';
        return -1;
    }
    pugi::xml_node log = root.child(CS_TAG_LOG);
    if (!log) {
        return 0;
    }
    pugi::xml_node pathFile = log.child(CS_TAG_PATH_FILE);
    if (!pathFile) {
        return 0;
    }
    path = pathFile.text().get();
    return 1;
}

Log::Log(const char *filePath)
{
    auto res = buildLog(filePath);
    if (res == -1) {
        std::cerr << "Error to create log.\n";
    }
    if (!res) {
        std::cerr << "Output file will be save in same directory with suffix _log.\n";
        path.append(filePath);
        size_t found = path.find_last_of(".");
        if (found != std::string::npos) {
            path.insert(found, "_log");
        } else {
            path.append("_log");
        }
    }
}