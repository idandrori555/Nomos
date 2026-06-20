#include "http.hpp"
#include "nomos.hpp"
#include "types.hpp"
#include <iostream>

int main()
{
  using namespace nomos;
  using namespace nomos::http;

  App app;

  // Simple logging middleware
  app.use([](const Request &req, auto &, auto next)
          {
            std::cout << "[" << req.method << "] " << req.path << std::endl;
            next();
          });

  app.use([](const auto &, auto &, auto next)
          {
            std::cout << "Second Middlware" << std::endl;
            next();
          });

  app.get("/", [](const auto &, Response &res)
          {
            res.status(200)
                .header("X-Framework", "Nomos")
                .header("X-Version", "0.1.0")
                .body("Hello from C++23!")
                .send();
          });

  // Serve static assets natively
  app.get("/html", [](const auto &, Response &res)
          {
            res.file("public/index.html").send();
          });

  app.all("/api", [](const auto &, Response &res)
          {
            res.status(200).body("ALL HIT!").send();
          });

  app.post("/api", [](const auto &, Response &res)
           {
             res.status(200).body("POST HIT!").send();
           });

  // Start listening on port 1234
  app.listen(1234, [](auto port)
             {
               std::cout << "🚀 Nomos server online at http://localhost:" << port
                         << std::endl;
             });

  return 0;
}
