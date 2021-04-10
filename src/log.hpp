#ifndef RRT_RRTstar_LOG_HPP
#define RRT_RRTstar_LOG_HPP

#include <iostream>
#include "lib/pugixml/src/pugixml.hpp"
#include "constants.hpp"

class Log
{
public:
    Log() = delete;
    explicit Log(const char *);
    ~Log() = default;

    inline std::string getPath() const
    {
        return path;
    }
private:
    std::string path;

    short buildLog(const char *);
};

#endif