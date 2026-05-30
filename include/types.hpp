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
#define NOMOS_IS_WINDOWS 0
using socket_t = int;
#elif defined(__WIN32) || defined(_WIN64)
#include <winsock2.h>
#define NOMOS_IS_WINDOWS 1
using socket_t = SOCKET;
#endif

typedef unsigned short port_t;
typedef std::function<void(port_t)> NomosListenCallback;
typedef std::function<void(const http::Request &, const http::Response &)> NomosHandler;

} // namespace nomos::types
