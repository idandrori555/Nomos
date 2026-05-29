#pragma once

#include "types.hpp"

namespace nomos::consts
{
constexpr const char *HTTP_END = "\r\n";
constexpr const char *HTTP_METHOD_GET = "GET";
constexpr const char *HTTP_METHOD_POST = "POST";
constexpr const char *HTTP_METHOD_ALL = "ALL";

#if NOMOS_IS_WINDOWS == 1
constexpr const types::socket_t INVALID_SOCKET = -1;
#else
constexpr const types::socket_t INVALID_SOCKET = -1;
#endif
} // namespace nomos::consts
