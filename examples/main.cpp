#include "../include/http.hpp"
#include "../include/nomos.hpp"
#include <iostream>

int main(void)
{
  nomos::App app;

  app.get("/", [](const auto &req, nomos::http::Response &res)
          {
            res.status(500).body("Hello World!").send();
          });

  app.listen(1234, [](auto port)
             {
               std::cout << "Listening on port " << port << std::endl;
             });

  return 0;
}
