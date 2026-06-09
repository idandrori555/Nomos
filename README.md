# Nomos 🚀

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

## 🧑‍💻 The Developer Experience

Writing backend APIs in C++ shouldn't feel like writing assembly. Nomos makes it incredibly clean:

```cpp
#include "nomos.hpp"
#include "http.hpp"
#include <iostream>

int main()
{
    nomos::App app;

    // Simple logging middleware
    app.use([](auto &req, auto &res) {
        std::cout << "[" << req.method << "] " << req.path << std::endl;
    });

    // Clean, fluent routing
    app.get("/", [](const auto &req, nomos::http::Response &res) {
        res.status(200)
           .header("X-Framework", "Nomos")
           .body("Hello from C++23!")
           .send();
    });

    // Serve static assets natively
    app.get("/dashboard", [](const auto &req, nomos::http::Response &res) {
        res.file("public/index.html").send();
    });

    // Start listening on port 1234
    app.listen(1234, [](auto port) {
        std::cout << "🚀 Nomos server online at http://localhost:" << port << std::endl;
    });

    return 0;
}
```

---

## 🚀 Features At A Glance

- **Ergonomic API** — Built with an intuitive, developer-first routing design.
- **Unified Cross-Platform Core** — Transparent support for Linux and Windows networking out of the box.
- **Embedded Thread Pool** — Handles multiple concurrent users automatically in the background.
- **Zero-Dependency Setup** — Standard CMake build with no external libraries required.

---

## ⚡ Quickstart

Getting up and running takes less than a minute.

### 1. Build the Server

```bash
# Clone the repository
git clone https://github.com/yourusername/nomos.git
cd nomos

# Configure and compile
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### 2. Run the Example

```bash
./build/nomos_example
```

Open `http://localhost:1234` in your browser and you're live!

---

## 📄 License

Nomos is open-source software licensed under the MIT License.
