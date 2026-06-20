#include "nomos.hpp"
#include "http.hpp"
#include "socket.hpp"
#include "types.hpp"

namespace nomos
{
void App::listen(port_t port, NomosListenCallback callback)
{
  internal::SocketEngine engine;

  auto listen_result = engine.listen(port);
  if (!listen_result.has_value())
    return;

  this->m_port = port;

  if (callback)
    callback(port);

  while (true)
  {
    socket_t client_fd = engine.accept_connection();
    if (client_fd == consts::INVALID_SOCKET)
      continue;

    m_thread_pool.enqueue([this, client_fd]()
                          {
                            std::string raw_request = internal::SocketEngine::read_request(client_fd);
                            http::Response res{client_fd};

                            if (auto req = http::HttpParser::parse(raw_request); req.has_value())
                            {
                              execute_all_middleware(*req, res);

                              bool route_found = false;

                              // Try matching the specific HTTP method first
                              if (auto method_it = m_routes.find(req->method); method_it != m_routes.end())
                              {
                                if (auto path_it = method_it->second.find(req->path); path_it != method_it->second.end())
                                {
                                  for (const auto &handler : path_it->second)
                                    handler(*req, res);

                                  route_found = true;
                                }
                              }

                              // Fall back to universal "ALL" routes if no specific method matched
                              if (!route_found)
                              {
                                if (auto all_it = m_routes.find(std::string(consts::HTTP_METHOD_ALL)); all_it != m_routes.end())
                                  if (auto path_it = all_it->second.find(req->path); path_it != all_it->second.end())
                                  {
                                    for (const auto &handler : path_it->second)
                                      handler(*req, res);

                                    route_found = true;
                                  }
                              }

                              // Send 404 if no path profiles matched
                              if (!route_found)
                                res.status(consts::HTTP_STATUS_NOT_FOUND).send("Not Found!");
                            }

                            internal::SocketEngine::close_connection(client_fd);
                          });
  }
}

void App::add_route(std::string_view method, std::string_view path, NomosHandler handler)
{
  m_routes[std::string(method)][std::string(path)].push_back(std::move(handler));
}

void App::execute_all_middleware(http::Request &req, http::Response &res)
{
  if (m_middleware.empty())
    return;

  auto dispatch = [&](auto &self, size_t idx) -> void
  {
    if (idx >= m_middleware.size())
      return;

    auto &current_middleware = m_middleware[idx];
    auto next = [idx, &self]()
    {
      self(self, idx + 1);
    };

    current_middleware(req, res, next);
  };

  dispatch(dispatch, 0);
}

void App::use(NomosMiddleware handler)
{
  m_middleware.push_back(std::move(handler));
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
