#include "../include/http.hpp"
#include "../include/nomos.hpp"
#include <iostream>
#include <print>

int main(void)
{
  using namespace nomos;

  nomos::App app;

  auto middlewareLogger = [](...)
  {
    std::println("REQUEST DETECTED!");
  };

  app.use(middlewareLogger);

  app.get("/", [](const auto &req, http::Response &res)
          {
            res.status(200).body("Hello World!").send();
          });

  app.listen(1234, [](auto port)
             {
               std::cout << "Listening on port " << port << std::endl;
             });

  return 0;
}
