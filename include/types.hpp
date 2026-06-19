#pragma once

#include <functional>
#include <string>

namespace nomos::http
{
struct Request;
class Response;
}; // namespace nomos::http

namespace nomos::types
{

#ifdef __linux__
#undef NOMOS_IS_WINDOWS
#elif defined(__WIN32) || defined(_WIN64)
#define NOMOS_IS_WINDOWS
#include <winsock2.h>
#endif

#ifdef NOMOS_IS_WINDOWS
using socket_t = SOCKET;
#else
using socket_t = int;
#endif

/// === Types ===
struct Header
{
  std::string key;
  std::string value;
};

using Headers = std::vector<Header>;
using status_t = unsigned short;
using port_t = unsigned short;

// === Function Types ===
using NomosListenCallback = std::function<void(port_t)>;
using NomosHandler = std::function<void(const http::Request &, http::Response &)>;
using NomosNextFunction = std::function<void(void)>;
using NomosMiddleware = std::function<void(http::Request &, http::Response &, NomosNextFunction)>;

} // namespace nomos::types
