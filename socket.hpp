#pragma once

#include "nomos.hpp"
#include <expected>
#include <string>

namespace nomos::internal
{
enum class SocketError
{
  CreateFailed,
  BindFailed,
  ListenFailed,
};

constexpr int MAX_CHUNK_SIZE = 4096;

struct SocketEngine
{
  int server_fd{-1};

  // init bind and listen
  std::expected<void, SocketError> listen(port_t port) noexcept;

  // accept connection
  int accept_connection(void) noexcept;

  // read request
  std::string read_request(int client_fd) const noexcept;

  // send response
  void send_response(int client_fd, std::string_view response) const noexcept;

  // close socket
  void close_connection(int fd) noexcept;
};

}; // namespace nomos::internal
