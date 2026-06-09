#include "http.hpp"
#include "nomos.hpp"
#include <iostream>

int main()
{
  using namespace nomos;

  App app;

  // Simple logging middleware
  app.use([](const auto &req, auto &)
          {
            std::cout << "[" << req.method << "] " << req.path << std::endl;
          });

  // Clean, fluent routing
  app.get("/", [](const auto &, nomos::http::Response &res)
          {
            res.status(200)
                .header("X-Framework", "Nomos")
                .body("Hello from C++23!")
                .send();
          });

  // Serve static assets natively
  app.get("/dashboard", [](const auto &, nomos::http::Response &res)
          {
            res.file("public/index.html").send();
          });

  // Start listening on port 1234
  app.listen(1234, [](auto port)
             {
               std::cout << "🚀 Nomos server online at http://localhost:" << port
                         << std::endl;
             });

  return 0;
}
