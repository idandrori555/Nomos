#include "../include/socket.hpp"

#ifdef NOMOS_IS_WINDOWS
#include <winsock2.h>
#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <array>

namespace nomos::internal
{
// init bind and listen
std::expected<void, SocketError> SocketEngine::listen(port_t port) noexcept
{
#ifdef NOMOS_IS_WINDOWS
  WSADATA wsa_data;
  if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
  {
    return std::unexpected(SocketError::CreateFailed);
  }
#endif
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == consts::INVALID_SOCKET)
    return std::unexpected(SocketError::CreateFailed);

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);

  if (bind(server_fd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0)
  {
    close_connection(server_fd);
    return std::unexpected(SocketError::BindFailed);
  }

  if (::listen(server_fd, SOMAXCONN) < 0)
    return std::unexpected(SocketError::ListenFailed);

  return {};
};

// accept connection
socket_t SocketEngine::accept_connection(void) noexcept
{
  sockaddr_in client_addr{};
#ifdef NOMOS_IS_WINDOWS
  int client_addr_len = sizeof(client_addr);
#else
  socklen_t client_addr_len = sizeof(client_addr);
#endif
  return accept(server_fd, reinterpret_cast<struct sockaddr *>(&client_addr), &client_addr_len);
};

// read request
std::string SocketEngine::read_request(socket_t client_fd)
{
  std::array<char, consts::MAX_CHUNK_SIZE> buffer{};
#ifdef NOMOS_IS_WINDOWS
  long bytes_read = recv(client_fd, buffer.data(), static_cast<int>(buffer.size() - 1), 0);
#else
  long bytes_read = read(client_fd, buffer.data(), buffer.size() - 1);
#endif
  if (bytes_read <= 0)
    return "";

  return std::string(buffer.data(), bytes_read);
};

// send response
void SocketEngine::send_response(socket_t client_fd, std::string_view response)
{
#ifdef NOMOS_IS_WINDOWS
  send(client_fd, response.data(), static_cast<int>(response.size()), 0);
#else
  write(client_fd, response.data(), response.size());
#endif
}

// close socket
void SocketEngine::close_connection(socket_t fd)
{
  if (fd != consts::INVALID_SOCKET)
  {
#ifdef NOMOS_IS_WINDOWS
    closesocket(fd);
#else
    close(fd);
#endif
  }
}
}; // namespace nomos::internal
