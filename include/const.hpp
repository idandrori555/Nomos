#pragma once

#include "types.hpp"
#include <array>
#include <string>
#include <string_view>

namespace nomos::consts
{
constexpr std::string HTTP_END = "\r\n";
constexpr std::string HTTP_METHOD_GET = "GET";
constexpr std::string HTTP_METHOD_POST = "POST";
constexpr std::string HTTP_METHOD_ALL = "ALL";

constexpr types::status_t HTTP_STATUS_OK = 200;
constexpr types::status_t HTTP_STATUS_BAD_REQUEST = 400;
constexpr types::status_t HTTP_STATUS_NOT_FOUND = 404;
constexpr types::status_t HTTP_STATUS_INTERNAL_SERVER_ERROR = 500;

constexpr std::array<types::status_t, 4> HTTP_STATUS_CODES = {HTTP_STATUS_OK, HTTP_STATUS_BAD_REQUEST, HTTP_STATUS_NOT_FOUND, HTTP_STATUS_INTERNAL_SERVER_ERROR};

constexpr std::array<std::string_view, 20> HTTP_HEADERS = {
    "Accept",
    "Accept-Encoding",
    "Accept-Language",
    "Access-Control-Allow-Headers",
    "Access-Control-Allow-Methods",
    "Access-Control-Allow-Origin",
    "Authorization",
    "Cache-Control",
    "Connection",
    "Content-Encoding",
    "Content-Length",
    "Content-Type",
    "Cookie",
    "Date",
    "Host",
    "Location",
    "Origin",
    "Server",
    "Set-Cookie",
    "User-Agent"};

constexpr int MAX_CHUNK_SIZE = 4096;
constexpr int MAX_THREADS = 10;

constexpr const types::socket_t INVALID_SOCKET = -1;
} // namespace nomos::consts
