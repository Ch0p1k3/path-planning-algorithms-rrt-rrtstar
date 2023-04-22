#pragma once

#include <iostream>
#include <pugixml.hpp>

#include <constants.hpp>

class Log {
 public:
  Log() = delete;
  explicit Log(const char*);
  ~Log() = default;

  inline std::string getPath() const {
    return path;
  }

 private:
  std::string path;

  short buildLog(const char*);
};
