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
  static bool parse_start_line(std::string_view line, std::string_view &method, std::string_view &path, std::string_view &version) noexcept;
  static Headers parse_headers(std::string_view header_section) noexcept;
  static std::optional<Request> parse(std::string_view raw_http) noexcept;
};
} // namespace nomos::http
