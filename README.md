# Nomos

A lightweight C++ HTTP server framework with a thread pool, simple routing, and cross platform socket support.

Built for learning and experimenting with low level networking, concurrency, and minimal web frameworks.

---

## Features

- Simple HTTP server with routing system
- Thread pool for handling concurrent connections
- Cross platform socket abstraction (Windows + Linux)
- Basic HTTP request parser
- Handler based routing (GET, POST, ALL)
- Clean separation between networking, HTTP, and application layers

---

## Architecture Overview

The project is split into 4 main parts:

### 1. Socket Layer
Handles raw TCP networking:
- Create server socket
- Bind and listen on port
- Accept client connections
- Read and write raw data
- Cross platform support via preprocessor checks

### 2. HTTP Layer
Parses and responds to HTTP requests:
- Simple request line parser (method, path, version)
- Minimal response builder
- Sends HTTP/1.1 responses over socket

### 3. Thread Pool
Manages concurrency:
- Pre-allocated worker threads
- Task queue protected by mutex
- Condition variable for wakeups
- Stop token support for clean shutdown

### 4. App Layer
User facing API:
- Route registration (GET, POST, ALL)
- Request dispatching
- Thread pool integration
- Handler execution per request

---

## Example Usage

```cpp
#include "../include/nomos.hpp"
#include "../include/http.hpp"
#include <iostream>

int main()
{
  nomos::App app;

  app.get("/", [](const auto& req, const auto& res)
  {
    res.send("Hello World!");
  });

  app.listen(1234, [](auto port)
  {
    std::cout << "Listening on port " << port << std::endl;
  });

  return 0;
}
