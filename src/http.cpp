#include "http.hpp"
#include "const.hpp"
#include "socket.hpp"
#include <format>
#include <fstream>
#include <iostream>
#include <string_view>

namespace nomos::http
{
// Helper to extract the method, path, and version from the start line
bool HttpParser::parse_start_line(std::string_view line, std::string_view &method, std::string_view &path, std::string_view &version) noexcept
{
  size_t first_space = line.find(' ');
  if (first_space == std::string_view::npos)
    return false;

  size_t second_space = line.find(' ', first_space + 1);
  if (second_space == std::string_view::npos)
    return false;

  method = line.substr(0, first_space);
  path = line.substr(first_space + 1, second_space - (first_space + 1));
  version = line.substr(second_space + 1);

  return !method.empty() && !path.empty() && !version.empty();
}

// Helper to trim leading spaces from header values
std::string_view trim_leading_spaces(std::string_view str) noexcept
{
  while (!str.empty() && str.front() == ' ')
  {
    str.remove_prefix(1);
  }
  return str;
}

// Helper to populate the headers vector
Headers HttpParser::parse_headers(std::string_view header_section) noexcept
{
  Headers headers;
  size_t pos = 0;

  while (pos < header_section.size())
  {
    size_t next_line = header_section.find("\r\n", pos);
    std::string_view line = (next_line == std::string_view::npos)
                                ? header_section.substr(pos)
                                : header_section.substr(pos, next_line - pos);

    if (line.empty())
      break;

    size_t colon = line.find(':');
    if (colon != std::string_view::npos)
    {
      std::string_view key = line.substr(0, colon);
      std::string_view value = trim_leading_spaces(line.substr(colon + 1));

      headers.push_back({std::string(key), std::string(value)});
    }

    if (next_line == std::string_view::npos)
      break;

    pos = next_line + 2;
  }

  return headers;
}

std::optional<Request> HttpParser::parse(std::string_view raw_http) noexcept
{
  if (raw_http.empty())
    return std::nullopt;

  // Split headers and body
  size_t header_end = raw_http.find("\r\n\r\n");
  std::string_view header_section = (header_end == std::string_view::npos) ? raw_http : raw_http.substr(0, header_end);
  std::string_view body = (header_end == std::string_view::npos) ? "" : raw_http.substr(header_end + 4);

  // Extract the first line
  size_t first_line_end = header_section.find("\r\n");
  if (first_line_end == std::string_view::npos)
    return std::nullopt;

  std::string_view start_line = header_section.substr(0, first_line_end);

  // Parse components
  std::string_view method, path, version;
  if (!parse_start_line(start_line, method, path, version))
    return std::nullopt;

  // Parse the remaining headers
  std::string_view remaining_headers = header_section.substr(first_line_end + 2);
  Headers headers = parse_headers(remaining_headers);

  return Request{std::string(method), std::string(path), std::string(version), std::string(body), headers};
}

Response::Response(types::socket_t client_fd) noexcept : m_client_fd(client_fd), m_status(consts::HTTP_STATUS_OK)
{
  // Default headers
  m_headers.push_back({"Server", "Nomos"});
  m_headers.push_back({"Content-Type", "text/html"});
  m_headers.push_back({"Connection", "close"});
}

std::string Response::get_headers_str(const types::Headers &headers) noexcept
{
  std::string result;

  for (const auto &header : headers)
    result += std::format("{}: {}\r\n", header.key, header.value);

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

std::string_view Response::get_content_type(std::string_view file_path) noexcept
{
  constexpr std::string_view DEFAULT_CONTENT_TYPE = "text/html";

  if (file_path.ends_with(".css"))
    return "text/css";

  else if (file_path.ends_with(".js"))
    return "application/javascript";

  else if (file_path.ends_with(".html"))
    return "text/html";
  else
    return DEFAULT_CONTENT_TYPE;
}

Response &Response::status(types::status_t status) noexcept
{
  m_status = status;
  return *this;
}

Response &Response::headers(const types::Headers &headers) noexcept
{
  for (const auto &header : headers)
    m_headers.push_back(header);

  return *this;
}

Response &Response::header(const std::string &key, const std::string &value) noexcept
{
  if (key.empty() || value.empty())
    return *this;

  for (auto &header : m_headers)
  {
    if (header.key == key)
    {
      // Update existing header
      header.value = value;
      return *this;
    }
  }

  m_headers.push_back({key, value});
  return *this;
}

Response &Response::body(std::string body) noexcept
{
  m_body = std::move(body);
  return *this;
}

Response &Response::file(std::string_view file_path) noexcept
{
  if (file_path.empty())
    return *this;

  if (file_path.contains(".."))
    return *this;

  // Set correct content type
  header("Content-Type", std::string{get_content_type(file_path)});

  std::ifstream file{std::string(file_path)};
  if (!file.is_open() || !file.good())
  {
    std::cerr << "Error opening file: " << file_path << std::endl;
    return *this;
  }

  m_body = std::string{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
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
