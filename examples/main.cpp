#include "../include/http.hpp"
#include "../include/nomos.hpp"
#include <iostream>

int main(void)
{
  using namespace nomos;

  nomos::App app;

  app.use([](auto &&...)
          {
            std::cout << "New Request Detected!" << std::endl;
          });

  app.get("/", [](const auto &, http::Response &res)
          {
            res.status(200).body("Hello World!").header("X-hello-world", "HELLO!!!!").send();
          });

  app.get("/file", [](const auto &, http::Response &res)
          {
            res.file("public/index.html").send();
          });

  app.listen(1234, [](auto port)
             {
               std::cout << "Listening on port " << port << std::endl;
             });

  return 0;
}
