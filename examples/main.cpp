#include "../include/http.hpp"
#include "../include/nomos.hpp"
#include <iostream>

int main(void)
{
  using namespace nomos;

  nomos::App app;

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
