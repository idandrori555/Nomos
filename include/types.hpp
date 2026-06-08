#pragma once

#include <functional>

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

using port_t = unsigned short;
using NomosListenCallback = std::function<void(port_t)>;
using NomosHandler = std::function<void(const http::Request &, http::Response &)>;
using NomosMiddleware = std::function<void(http::Request &, http::Response &)>;
using status_t = unsigned short;

} // namespace nomos::types
