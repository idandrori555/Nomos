#include "../include/http.hpp"
#include "../include/const.hpp"
#include "../include/socket.hpp"
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

void Response::send(std::string_view body) const noexcept
{
  internal::SocketEngine engine;

  std::string full_response = std::format(
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain\r\n"
      "Content-Length: {}\r\n"
      "Connection: close\r\n\r\n"
      "{}",
      body.size(), body);

  engine.send_response(m_client_fd, full_response);
};
}; // namespace nomos::http
