#include "socket.hpp"
#include <array>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace nomos::internal
{
// init bind and listen
std::expected<void, SocketError> SocketEngine::listen(port_t port) noexcept
{
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0)
    return std::unexpected(SocketError::CreateFailed);

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);

  if (bind(server_fd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0)
    return std::unexpected(SocketError::BindFailed);

  if (::listen(server_fd, SOMAXCONN) < 0)
    return std::unexpected(SocketError::ListenFailed);

  return {};
};

// accept connection
int SocketEngine::accept_connection(void) noexcept
{
  sockaddr_in client_addr{};
  socklen_t client_addr_len = sizeof(client_addr);
  return accept(server_fd, reinterpret_cast<struct sockaddr *>(&client_addr), &client_addr_len);
};

// read request
std::string SocketEngine::read_request(int client_fd) const noexcept
{
  std::array<char, MAX_CHUNK_SIZE> buffer{};
  long bytes_read = read(client_fd, buffer.data(), buffer.size() - 1);
  if (bytes_read <= 0)
    return "";

  return std::string(buffer.data(), bytes_read);
};

// send response
void SocketEngine::send_response(int client_fd, std::string_view response) const noexcept
{
  write(client_fd, response.data(), response.size());
};

// close socket
void SocketEngine::close_connection(int fd) noexcept
{
  close(fd);
}

}; // namespace nomos::internal
