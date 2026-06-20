#pragma once

#include <string>

namespace nomos::http
{
class Request
{
public:
  std::string method;
  std::string path;
  std::string version;
};
} // namespace nomos::http
