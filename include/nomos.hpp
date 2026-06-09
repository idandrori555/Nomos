#pragma once

#include "const.hpp"
#include "thread_pool.hpp"
#include "types.hpp"
#include <map>
#include <string_view>

namespace nomos
{
using namespace nomos::types;

class App
{
public:
  App(void) noexcept = default;
  ~App(void) = default;

  void listen(port_t port, NomosListenCallback callback = (NomosListenCallback) nullptr);

  void use(NomosMiddleware);

  void get(std::string_view path, NomosHandler);
  void post(std::string_view path, NomosHandler);
  void all(std::string_view path, NomosHandler);

private:
  std::vector<NomosMiddleware> m_middleware;
  std::map<std::string, std::vector<NomosHandler>> m_routes;
  port_t m_port{consts::INVALID_PORT};
  internal::ThreadPool m_thread_pool{consts::MAX_THREADS};

  void add_route(std::string_view method, std::string_view path, NomosHandler handler);
  void execute_all_middleware(http::Request &req, http::Response &res);
};
}; // namespace nomos
