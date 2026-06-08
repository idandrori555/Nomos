#include "../include/http.hpp"
#include "../include/nomos.hpp"
#include <iostream>
#include <print>

int main(void)
{
  using namespace nomos;

  nomos::App app;

  app.use([](...)
          {
            std::println("New Request Detected!");
          });

  app.get("/", [](const auto &, http::Response &res)
          {
            res.status(200).body("Hello World!").header("X-hello-world", "HELLO!!!!").send();
          });

  app.listen(1234, [](auto port)
             {
               std::cout << "Listening on port " << port << std::endl;
             });

  return 0;
}
