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
    if (client_fd == consts::INVALID_SOCKET)
      continue;

    // Hand off the connection descriptor directly to the pre-allocated thread pool
    m_thread_pool.enqueue([this, client_fd]()
                          {
                            std::string raw_request = internal::SocketEngine::read_request(client_fd);
                            http::Response res{client_fd};

                            if (auto req = http::HttpParser::parse(raw_request); req.has_value())
                            {
                              std::string specific_key = std::string(req->method) + " " + std::string(req->path);
                              std::string generic_key = "ALL " + std::string(req->path);

                              if (m_routes.contains(specific_key))
                              {
                                for (const auto &handler : m_routes.at(specific_key))
                                {
                                  handler(*req, res);
                                }
                              }
                              else if (m_routes.contains(generic_key))
                              {
                                for (const auto &handler : m_routes.at(generic_key))
                                {
                                  handler(*req, res);
                                }
                              }
                              else
                              {
                                // default unspecified route behavior
                                res.send("Not Found!");
                              }
                            }
                            internal::SocketEngine::close_connection(client_fd);
                          });
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
