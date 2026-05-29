#pragma once

#include "types.hpp"
#include <optional>
#include <string_view>

namespace nomos::http
{
struct Request
{
  std::string_view method;
  std::string_view path;
  std::string_view version;
  // TODO: add body, headers, etc..
};

class Response
{
private:
  nomos::types::socket_t m_client_fd;

public:
  explicit Response(int client_fd) noexcept : m_client_fd(client_fd)
  {
  }

  void send(std::string_view response) const noexcept;
};

class HttpParser
{
public:
  static std::optional<Request> parse(std::string_view raw_http) noexcept;
};

}; // namespace nomos::http
