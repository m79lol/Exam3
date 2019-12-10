#include <iostream>
#include <thread>
#include <cstring>
#include <HashServer/HashServer.h>

void printHelp() {
  std::cout << std::endl;
  std::cout << "Use: app [port] [threads_count]" << std::endl;
  std::cout << "     [port] is listening port (1024-65535). Default value is 6767." << std::endl;
  std::cout << "     [threads_count] is number of CPUs (1 or more), that server will use. Default value is all CPUs." << std::endl;
  std::cout << std::endl;
};

int main(int argc, char * argv[]) {
  
  for (int i = 1; i < argc; ++i) {
    if (!strcmp("-h", argv[i]) || !strcmp("--help", argv[i])) {
      printHelp();
      return 0;
    }
  }
  
  int port = 6767;
  unsigned int threadsCount = std::thread::hardware_concurrency();
  threadsCount = threadsCount ? threadsCount : 1;
  
  if (argc > 1) {
    port = atoi(argv[1]);
    if (port <= 1024 || port > 65535) {
      std::cerr << "Exception: the port must be within (1024-65535)." << std::endl;
      printHelp();
      return 1;
    }
    
    if (argc > 2) {
      int tmp = atoi(argv[2]);
      if (tmp <= 0) {
        std::cerr << "Exception: the threads_count must be greater than 0." << std::endl;
        printHelp();
        return 1;
      }
      threadsCount = tmp;
    }
  }
  
  try {
    HashServer::Pointer server = HashServer::create(port);
    server->start(threadsCount);
    server->work();
  }
  catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
