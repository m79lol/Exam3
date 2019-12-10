#pragma once

#ifndef __HASH_SERVER_IMP_h
#define __HASH_SERVER_IMP_h

#include <memory>
#include <thread>
#include <list>
#include <boost/asio.hpp>

#include <HashServer/HashServer.h>

using boost::asio::ip::tcp;

class Connection : public std::enable_shared_from_this<Connection> {
public:
  typedef std::shared_ptr<Connection> Pointer;
  static const std::string delimiter;
  static const std::size_t shift;

private:
  tcp::socket m_socket;
  boost::asio::streambuf m_buf;

  Connection(boost::asio::io_context& ioContext);

public:
  tcp::socket& getSocket();
  void doWork();

  static Pointer create(boost::asio::io_context& ioContext);
};

class HashServerImp : public std::enable_shared_from_this<HashServerImp>, public HashServer {
private:
  typedef std::shared_ptr<std::thread> ThreadPtr;
  
  boost::asio::io_context m_ioContext;
  tcp::acceptor m_acceptor;
  std::list<ThreadPtr> m_threads;

  void startAccept();

public:
  typedef std::shared_ptr<HashServerImp> Pointer;

  HashServerImp(const short int &port);

  void start(const unsigned short int &countThreads) override;
  void work() override;
  void stop() override;

  virtual ~HashServerImp();
  
};

#endif