#include "../include/http.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("HTTP Parser - Valid Requests", "[http]")
{
  using nomos::http::HttpParser;

  SECTION("Standard GET request")
  {
    std::string_view raw = "GET /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";
    auto req = HttpParser::parse(raw);

    REQUIRE(req.has_value());
    CHECK(req->method == "GET");
    CHECK(req->path == "/index.html");
    CHECK(req->version == "HTTP/1.1");
  }

  SECTION("POST request with short path")
  {
    std::string_view raw = "POST / HTTP/1.1\r\n\r\n";
    auto req = HttpParser::parse(raw);

    REQUIRE(req.has_value());
    CHECK(req->method == "POST");
    CHECK(req->path == "/");
    CHECK(req->version == "HTTP/1.1");
  }
}

TEST_CASE("HTTP Parser - Invalid/Malformed Requests", "[http]")
{
  using nomos::http::HttpParser;

  SECTION("Empty raw string")
  {
    auto req = HttpParser::parse("");
    CHECK_FALSE(req.has_value());
  }

  SECTION("Missing HTTP line terminator")
  {
    auto req = HttpParser::parse("GET /index.html HTTP/1.1");
    CHECK_FALSE(req.has_value());
  }

  SECTION("Malformed request line missing spaces")
  {
    auto req = HttpParser::parse("GET/index.htmlHTTP/1.1\r\n\r\n");
    CHECK_FALSE(req.has_value());
  }

  SECTION("Incomplete tokens")
  {
    auto req = HttpParser::parse("GET  HTTP/1.1\r\n\r\n");
    CHECK_FALSE(req.has_value());
  }
}
