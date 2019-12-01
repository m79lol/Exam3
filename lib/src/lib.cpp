
#include <iostream>
#include <picosha2.h>
#include <hasher/hasher.h>

std::string Hash::calculate(const std::string &source) {
  return picosha2::hash256_hex_string(source);
}

Connection::Connection(boost::asio::io_context &ioContext)
  : m_socket(ioContext) {}

tcp::socket& Connection::getSocket() {
  return m_socket;
}

void Connection::doWork() {
  auto self(shared_from_this());

  boost::asio::async_read_until(
    m_socket, m_buf, "\r\n", 
    [this, self](boost::system::error_code ec, std::size_t received) {
#ifdef DEBUG
      std::cout << "async_read_until: " << ec.message() << "\n";
      std::cout << "received: " << received << std::endl;
#endif

      if (ec == boost::asio::error::eof) { // connection closed
        return;
      }
      
      std::istream is(&m_buf);
      std::string sourceString;
      std::getline(is, sourceString);

      sourceString.erase(sourceString.end() - 1);
      std::string hexStr = Hash::calculate(sourceString);
      hexStr += "\r\n";

#ifdef DEBUG      
      std::cout << "message: " << sourceString << std::endl;
      std::cout << "hash: " << hexStr << std::endl;
#endif
      
      m_socket.write_some(boost::asio::buffer(hexStr.c_str(), hexStr.size()));

      doWork();
    }
  );
}

Connection::Pointer Connection::create(boost::asio::io_context& ioContext) {
  return Connection::Pointer(new Connection(ioContext));
}

HashServer::HashServer(boost::asio::io_context& ioContext, const short &port)
  : m_acceptor(ioContext, tcp::endpoint(tcp::v4(), port)) {
  startAccept();
}

void HashServer::startAccept() {
  Connection::Pointer newConnection = Connection::create(
    m_acceptor.get_executor().context()
  );

  m_acceptor.async_accept(
    newConnection->getSocket(),
    [this, newConnection](
      const boost::system::error_code& error
    ) {
      if (!error) {
        newConnection->doWork();
      }

      startAccept();
    }
  );
}
