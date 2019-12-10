#pragma once

#ifndef __HASH_SERVER_h
#define __HASH_SERVER_h

#ifdef NO_EXPORTS
  #define DLL_API
#else 
  #ifdef _WIN32
    #ifdef HashServer_EXPORTS
      #define DLL_API __declspec(dllexport)
    #else
      #define DLL_API __declspec(dllimport)
    #endif
  #else
    #define DLL_API
  #endif
#endif

#include <memory>

class DLL_API HashServer {
public:
  typedef std::shared_ptr<HashServer> Pointer;
  static Pointer create(const short int &port);

  virtual void start(const unsigned short int &countThreads) = 0;
  virtual void work() = 0;
  virtual void stop() = 0;

  virtual ~HashServer();

protected:
  HashServer();
   
};

#endif