#pragma once

#include "types.hpp"
#include <string>
#include <string_view>

namespace nomos::http
{
class Response
{
private:
  types::socket_t m_client_fd;
  types::status_t m_status;
  std::string m_body;
  types::Headers m_headers;

  static std::string get_status_line(types::status_t status) noexcept;
  static std::string get_headers_str(const types::Headers &headers) noexcept;
  static std::string_view get_content_type(std::string_view file_path) noexcept;

public:
  Response(types::socket_t client_fd) noexcept;
  ~Response() noexcept = default;

  Response &status(types::status_t status) noexcept;
  Response &headers(const types::Headers &headers) noexcept;
  Response &header(const std::string &key, const std::string &value) noexcept;
  Response &body(std::string body) noexcept;
  Response &file(std::string_view file_path) noexcept;
  void send(std::string_view response = "") const noexcept;
};
} // namespace nomos::http
