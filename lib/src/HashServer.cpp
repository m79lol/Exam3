#include <HashServer/HashServer.h>
#include <HashServerImp.h>

HashServer::HashServer() {}

HashServer::~HashServer() {}

HashServer::Pointer HashServer::create(const short int &port) {
  HashServerImp::Pointer ptr = std::make_shared<HashServerImp>(port);
  return std::dynamic_pointer_cast<HashServer>(ptr);
}