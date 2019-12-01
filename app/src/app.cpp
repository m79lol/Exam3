#include <iostream>
#include <hasher/hasher.h>

int main() {
  try {
    boost::asio::io_context ioContext;
    HashServer server(ioContext, 6767);
    ioContext.run();
  }
  catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}