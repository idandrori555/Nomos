#include "../include/http.hpp"
#include "../include/const.hpp"
#include "../include/socket.hpp"
#include <algorithm>
#include <format>

namespace nomos::http
{
std::optional<Request> HttpParser::parse(std::string_view raw_http) noexcept
{
  if (raw_http.empty())
    return std::nullopt;

  size_t line_end = raw_http.find(consts::HTTP_END);
  if (line_end == std::string_view::npos)
    return std::nullopt;

  std::string_view line = raw_http.substr(0, line_end);
  if (!line.empty() && line.back() == '\r')
  {
    line.remove_suffix(1);
  }

  size_t first_space = line.find(' ');
  if (first_space == std::string_view::npos)
    return std::nullopt;

  size_t second_space = line.find(' ', first_space + 1);
  if (second_space == std::string_view::npos)
    return std::nullopt;

  std::string_view path = line.substr(first_space + 1, second_space - (first_space + 1));
  std::string_view method = line.substr(0, first_space);
  std::string_view version = line.substr(second_space + 1);

  if (path.empty() || method.empty() || version.empty())
    return std::nullopt;

  return Request{.method = method, .path = path, .version = version};
}

std::string Response::get_headers_str(const Headers &headers) noexcept
{
  std::string result;

  for (const auto &header : headers)
  {
    result += std::format("{}: {}\r\n", header.key, header.value);
  }

  return result;
}

std::string Response::get_status_line(types::status_t status) noexcept
{
  switch (status)
  {
  case consts::HTTP_STATUS_OK:
    return "HTTP/1.1 200 OK";

  case consts::HTTP_STATUS_BAD_REQUEST:
    return "HTTP/1.1 400 Bad Request";

  case consts::HTTP_STATUS_NOT_FOUND:
    return "HTTP/1.1 404 Not Found";

  case consts::HTTP_STATUS_INTERNAL_SERVER_ERROR:
    return "HTTP/1.1 500 Internal Server Error";

  default:
    return "HTTP/1.1 500 Internal Server Error";
  }
}

bool Response::is_valid_header(std::string_view header) noexcept
{
  return std::ranges::contains(consts::HTTP_HEADERS, header);
}

Response &Response::status(types::status_t status) noexcept
{
  m_status = status;
  return *this;
}

Response &Response::headers(const Headers &headers) noexcept
{
  for (const auto &header : headers)
  {
    if (!is_valid_header(header.key))
    {
    }
    continue;

    m_headers.push_back(header);
  }
  return *this;
}

Response &Response::header(const std::string &key, const std::string &value) noexcept
{
  if (!is_valid_header(key))
    return *this;

  m_headers.push_back({.key = key, .value = value});
  return *this;
}

Response &Response::body(std::string_view body) noexcept
{
  m_body = body;
  return *this;
}

void Response::send(std::string_view response) const noexcept
{
  std::string_view real_body;
  if (!response.empty())
    real_body = response;
  else
    real_body = m_body;

  std::string full_response = std::format(
      "{}\r\n"                 // Status line here
      "Content-Length: {}\r\n" // Content-Length here
      "{}\r\n"                 // Rest of the headers here
      "{}",                    // Body here
      get_status_line(m_status),
      real_body.size(),
      get_headers_str(m_headers),
      real_body);

  internal::SocketEngine::send_response(m_client_fd, full_response);
};
}; // namespace nomos::http
