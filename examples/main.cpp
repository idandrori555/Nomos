#include "../include/nomos.hpp"
#include <print>

int main(void)
{
  nomos::App app;

  app.get("/", [](auto req, auto res)
          {
            res.send("Hello World!");
          });

  app.listen(1234, [](auto port)
             {
               std::println("Listening on port {}", port);
             });

  return 0;
}
