#pragma once

#include "const.hpp"
#include "types.hpp"
#include <optional>
#include <string>
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

struct Header
{
  std::string key;
  std::string value;
};

using Headers = std::vector<Header>;

class Response
{
private:
  types::socket_t m_client_fd;
  types::status_t m_status;
  std::string m_body;
  Headers m_headers;
  bool m_committed;

  static std::string get_status_line(types::status_t status) noexcept;
  static std::string get_headers_str(const Headers &headers) noexcept;

public:
  Response(types::socket_t client_fd) noexcept : m_client_fd(client_fd), m_status(consts::HTTP_STATUS_OK), m_committed(false)
  {
    m_headers.emplace_back("Server", "Nomos");
    m_headers.emplace_back("Content-Type", "text/html");
    m_headers.emplace_back("Connection", "close");
  }

  ~Response() noexcept = default;

  Response &status(types::status_t status) noexcept;
  Response &headers(const Headers &headers) noexcept;
  Response &header(const std::string &key, const std::string &value) noexcept;
  Response &body(std::string_view body) noexcept;
  void send(std::string_view response = "") const noexcept;
};

class HttpParser
{
public:
  static std::optional<Request> parse(std::string_view raw_http) noexcept;
};

}; // namespace nomos::http
