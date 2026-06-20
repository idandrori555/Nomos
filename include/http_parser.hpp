#pragma once

#include "request.hpp"
#include <optional>
#include <string_view>

namespace nomos::http
{
class HttpParser
{
  using Request = nomos::http::Request;

public:
  static std::optional<Request> parse(std::string_view raw_http) noexcept;
};
} // namespace nomos::http
