#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <pugixml.hpp>
#include "searchresult.hpp"
#include "constants.hpp"

namespace OutData
{
    void fill(const std::string&, const SearchResult &);

    void fillFirst(const std::string&, const SearchResult &);
}
