#pragma once

#include "types.hpp"
#include <string_view>

namespace nomos::consts
{
constexpr std::string_view HTTP_END = "\r\n";
constexpr std::string_view HTTP_METHOD_GET = "GET";
constexpr std::string_view HTTP_METHOD_POST = "POST";
constexpr std::string_view HTTP_METHOD_ALL = "ALL";

constexpr types::status_t HTTP_STATUS_OK = 200;
constexpr types::status_t HTTP_STATUS_BAD_REQUEST = 400;
constexpr types::status_t HTTP_STATUS_NOT_FOUND = 404;
constexpr types::status_t HTTP_STATUS_INTERNAL_SERVER_ERROR = 500;

constexpr int MAX_CHUNK_SIZE = 4096;
constexpr int MAX_THREADS = 10;

constexpr const types::port_t INVALID_PORT = 0;
constexpr const types::socket_t INVALID_SOCKET = -1;

auto constexpr NomosEmptyLambda = [](void)
{
};
} // namespace nomos::consts
