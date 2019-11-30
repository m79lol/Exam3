#pragma once

#ifndef __HASH_SERVER_h
#define __HASH_SERVER_h

#ifdef _WIN32
  #ifdef DLL_API_EXPORT
    #define DLL_API __declspec(dllexport)
  #else
    #define DLL_API __declspec(dllimport)
  #endif
#else
  #define RCML_SDK_API
#endif

#include <memory>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class DLL_API Connection : public std::enable_shared_from_this<Connection> {
public:
  typedef std::shared_ptr<Connection> Pointer;

private:
  tcp::socket m_socket;
  boost::asio::streambuf m_buf;

  Connection(boost::asio::io_context& ioContext);

public:
  tcp::socket& getSocket();
  void doWork();

  static Pointer create(boost::asio::io_context& ioContext);
};

class DLL_API HashServer {
private:
  tcp::acceptor m_acceptor;

public:
  HashServer(boost::asio::io_context& ioContext, const short &port);

private:
  void startAccept();
};

#endif