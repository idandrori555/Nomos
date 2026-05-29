#pragma once

#include "const.hpp"
#include "types.hpp"
#include <expected>
#include <string>

namespace nomos::internal
{
using namespace nomos::types;

enum class SocketError
{
  CreateFailed,
  BindFailed,
  ListenFailed,
};

struct SocketEngine
{
  socket_t server_fd{consts::INVALID_SOCKET};

  // init bind and listen
  std::expected<void, SocketError> listen(port_t port) noexcept;

  // accept connection
  socket_t accept_connection(void) noexcept;

  // read request
  std::string read_request(socket_t client_fd) const noexcept;

  // send response
  void send_response(socket_t client_fd, std::string_view response) const noexcept;

  // close socket
  void close_connection(socket_t fd) noexcept;
};

}; // namespace nomos::internal
