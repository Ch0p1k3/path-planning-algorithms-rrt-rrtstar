#pragma once

#include <iostream>
#include <pugixml.hpp>
#include <string>
#include <vector>

#include <constants.hpp>
#include <searchresult.hpp>

namespace OutData {
void fill(const std::string&, const SearchResult&);

void fillFirst(const std::string&, const SearchResult&);
}  // namespace OutData
