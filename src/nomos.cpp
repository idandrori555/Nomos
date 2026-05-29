#include "../include/nomos.hpp"
#include "../include/const.hpp"
#include "../include/http.hpp"
#include "../include/socket.hpp"

namespace nomos
{
void App::listen(port_t port, NomosListenCallback callback)
{
  internal::SocketEngine engine;
  if (!engine.listen(port))
    return;

  this->m_port = port;

  if (callback)
    callback(port);

  while (true)
  {
    socket_t client_fd = engine.accept_connection();
    if (client_fd == internal::INVALID_SOCKET)
      continue;

    std::string raw_request = engine.read_request(client_fd);
    http::Response res{client_fd};

    if (auto req = http::HttpParser::parse(raw_request); req.has_value())
    {
      std::string specific_key = std::string(req->method) + " " + std::string(req->path);
      std::string generic_key = "ALL " + std::string(req->path);

      if (m_routes.contains(specific_key)) // Check if there is a specific route for this request. (GET, POST, etc...)
      {
        for (const auto &handler : m_routes[specific_key]) // Go over all middleware functions and call them with request and response objects.
        {
          handler(*req, res);
        }
      }
      else if (m_routes.contains(generic_key)) // Check if there is a generic route for this request. (ALL)
      {
        for (const auto &handler : m_routes[generic_key]) // Go over all middleware functions and call them with request and response objects.
        {
          handler(*req, res);
        }
      }
      else
      {
        res.send("Not Found!");
      };
    }

    engine.close_connection(client_fd);
  }
}
void App::add_route(std::string_view method, std::string_view path, NomosHandler handler)
{
  std::string key = std::string(method) + " " + std::string(path);
  m_routes[key].push_back(std::move(handler));
}

void App::get(std::string_view path, NomosHandler handler)
{
  add_route(consts::HTTP_METHOD_GET, path, std::move(handler));
}
void App::post(std::string_view path, NomosHandler handler)
{
  add_route(consts::HTTP_METHOD_POST, path, std::move(handler));
}
void App::all(std::string_view path, NomosHandler handler)
{
  add_route(consts::HTTP_METHOD_ALL, path, std::move(handler));
}

}; // namespace nomos
