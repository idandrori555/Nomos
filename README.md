# Nomos 🚀

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue)](https://en.cppreference.com/w/cpp/23)
[![License: MIT](https://img.shields.io/badge/License-MIT-green)](LICENSE)
[![Linux Build](https://img.shields.io/badge/Linux-passing-brightgreen)](https://github.com/idandrori555/Nomos/actions)
[![Windows Build](https://img.shields.io/badge/Windows-passing-brightgreen)](https://github.com/idandrori555/Nomos/actions)

**Express-like simplicity. Bare-metal C++ speed.**

Nomos is an ultra-lightweight, high-performance web framework for C++23. It strips away the heavy boilerplate and complex enterprise architectures of traditional C++ servers, giving you a clean, expressive API that feels like modern JavaScript or Go, backed by the raw speed of native code.

No complex configurations. No massive dependencies. Just fast, elegant APIs.

---

## Why Nomos?

### ✨ Expressive Routing
Write clean, readable route handlers without wading through endless configuration templates.

### 🔌 Plug-and-Play Middleware
Chain logging, authentication, and request handling flows sequentially with zero fuss.

### 📁 Instant Static File Serving
Serve HTML, CSS, and assets directly to your users with a single, elegant command.

### ⚡ No Bloat, No Dependencies
Standard, pure C++ compilation that runs seamlessly on both Linux and Windows.

---

## The Developer Experience

Writing backend APIs in C++ shouldn't feel like writing assembly. Nomos makes it incredibly clean:

```cpp
#include "nomos.hpp"
#include "http.hpp"
#include <iostream>

int main()
{
    nomos::App app;

    // Simple logging middleware
    app.use([](auto &req, auto &res, auto next) {
        std::cout << "[" << req.method << "] " << req.path << std::endl;
        next();
    });

    // Clean, fluent routing
    app.get("/", [](const auto &req, nomos::http::Response &res, auto next) {
        res.status(200)
           .header("X-Framework", "Nomos")
           .body("Hello from C++23!")
           .send();
    });

    // Serve static assets natively
    app.get("/dashboard", [](const auto &req, nomos::http::Response &res, auto next) {
        res.file("public/index.html").send();
    });

    // Start listening on port 1234
    app.listen(1234, [](auto port) {
        std::cout << "Nomos server online at http://localhost:" << port << std::endl;
    });

    return 0;
}
```

---

## Features

| Feature | Status |
|---------|--------|
| Routing (`GET`, `POST`, `ALL`) | ✅ |
| Middleware pipeline | ✅ |
| Static file serving | ✅ |
| Thread pool (automatic concurrency) | ✅ |
| Cross-platform (Linux + Windows) | ✅ |
| Fluent response builder | ✅ |
| Route parameters (`/users/:id`) | 🔜 Planned |
| Request header / cookie / query accessors | 🔜 Planned |
| JSON body parsing | ❌ Not yet |
| HTTPS / TLS | ❌ Not yet |

---

## Requirements

- **Compiler:** GCC 14+, Clang 18+, or MSVC 2022+ (C++23 support required)
- **CMake:** 3.25+
- **No external runtime dependencies** (Catch2 is fetched automatically for tests)

---

## Quickstart

### 1. Build

```bash
git clone https://github.com/idandrori555/Nomos.git
cd Nomos

# Release build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Debug build (for development)
cmake -B build_debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build_debug
```

### 2. Run the Example

```bash
./build/nomos_example
```

Open `http://localhost:1234` in your browser and you're live!

### 3. Run the Tests

```bash
cmake --build build
./build/nomos_tests

# Or via CTest
ctest --test-dir build
```

---

## API Overview

### `App`

| Method | Description |
|--------|-------------|
| `get(path, handler)` | Register a GET route |
| `post(path, handler)` | Register a POST route |
| `all(path, handler)` | Register a catch-all method route |
| `use(middleware)` | Add a middleware function |
| `listen(port, callback)` | Start the server |

### `Response`

Fluent builder returned from handlers:

```cpp
res.status(200)
   .header("X-Custom", "value")
   .body("Hello")
   .send();
```

### `Request`

Data object with fields: `method`, `path`, `version`, `body`, `headers`.

---

## Contributing

Contributions are welcome! See [TASKS.txt](TASKS.txt) for open items.

- **Code style:** C++23, no exceptions, match existing patterns
- **PRs:** Keep changes focused; include tests where possible
- **Issues:** Please check existing issues before opening a new one

---

## Roadmap

- [ ] Route parameters (`/users/:id`)
- [ ] Request header / cookie / query string accessors
- [ ] JSON body parsing
- [ ] Chunked transfer encoding & keep-alive
- [ ] HTTPS support
- [ ] CI pipeline (GitHub Actions)

---

## License

Nomos is open-source software licensed under the [MIT License](LICENSE).
