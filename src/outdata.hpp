#ifndef RRT_RRTstar_OUT_DATA_HPP
#define RRT_RRTstar_OUT_DATA_HPP

#include <iostream>
#include <vector>
#include <string>
#include "lib/pugixml/src/pugixml.hpp"
#include "searchresult.hpp"
#include "constants.hpp"

namespace OutData
{
    void fill(const char *, const SearchResult &);
}

#endif