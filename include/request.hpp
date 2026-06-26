#pragma once

#include "types.hpp"
#include <string>

namespace nomos::http
{
using namespace nomos::types;
class Request
{

public:
  std::string method;
  std::string path;
  std::string version;
  std::string body;
  Headers headers;

  Request(std::string method, std::string path, std::string version, std::string body, Headers headers) noexcept
      : method(std::move(method)), path(std::move(path)), version(std::move(version)), body(std::move(body)), headers(std::move(headers))
  {
  }
};
} // namespace nomos::http
