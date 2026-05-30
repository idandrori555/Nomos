#pragma once

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

  void listen(port_t port, NomosListenCallback callback);

  void get(std::string_view path, NomosHandler);
  void post(std::string_view path, NomosHandler);
  void all(std::string_view path, NomosHandler);

private:
  std::map<std::string, std::vector<NomosHandler>> m_routes;
  port_t m_port{0};

  void add_route(std::string_view method, std::string_view path, NomosHandler handler);
};
}; // namespace nomos
