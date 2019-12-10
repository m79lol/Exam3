
#include <iostream>
#include <picosha2.h>

#include <HashServerImp.h>

#ifdef _WIN32
const std::string Connection::delimiter = "\r\n";
#elif __linux__
const std::string Connection::delimiter = "\n";
#endif
const std::size_t Connection::shift = Connection::delimiter.length();

Connection::Connection(boost::asio::io_context &ioContext)
  : m_socket(ioContext) {}

tcp::socket& Connection::getSocket() {
  return m_socket;
}

void Connection::doWork() {
  auto self(shared_from_this());

  boost::asio::async_read_until(
    m_socket, m_buf, delimiter,
    [this, self](boost::system::error_code ec, std::size_t received) {
#ifdef DEBUG
      std::cout << "async_read_until: " << ec.message() << "\n";
      std::cout << "received: " << received << std::endl;
#endif

      if (ec == boost::asio::error::eof) { // connection closed
        return;
      }

      const std::size_t payloadLen = received - shift;

      static const int buffer_size = PICOSHA2_BUFFER_SIZE_FOR_INPUT_ITERATOR;
      std::vector<picosha2::byte_t> buffer(buffer_size);
      picosha2::hash256_one_by_one hasher;

      const unsigned char *first = static_cast<const unsigned char *>(m_buf.data().data());
      for (std::size_t k = 0; k < payloadLen; ) {
        int size = buffer_size;
        for (int i = 0; i != buffer_size; ++i, ++first, ++k) {
          if (k >= payloadLen) {
            size = i;
            break;
          }
          buffer[i] = *first;
        }
        hasher.process(buffer.begin(), buffer.begin() + size);
      }
      hasher.finish();

      picosha2::byte_t hashed[picosha2::k_digest_size];
      hasher.get_hash_bytes(hashed, hashed + picosha2::k_digest_size);
      std::ostringstream oss;
      picosha2::output_hex(hashed, hashed + picosha2::k_digest_size, oss);

      std::string hexStr;
      hexStr.assign(oss.str());
      hexStr += delimiter;

#ifdef DEBUG
      std::cout << "hash: " << hexStr << std::endl;
#endif

      m_socket.write_some(boost::asio::buffer(hexStr.c_str(), hexStr.size()));

      m_buf.consume(received);

      doWork();
    }
  );
}

Connection::Pointer Connection::create(boost::asio::io_context& ioContext) {
  return Connection::Pointer(new Connection(ioContext));
}

HashServerImp::HashServerImp(const short int &port)
  : HashServer(), m_acceptor(m_ioContext, tcp::endpoint(tcp::v4(), port)) {
  startAccept();
}

HashServerImp::~HashServerImp() {
  stop();
}

void HashServerImp::start(const unsigned short int &countThreads) {
  auto self(shared_from_this());

  for (unsigned short int i = 0; i < countThreads; ++i) {
    m_threads.push_back(ThreadPtr( 
      new std::thread(
        [this, self]() mutable -> void {
            self->m_ioContext.run();
        }
      )
    ));
  }
}

void HashServerImp::work() {
  for (auto iThread : m_threads) {
    iThread->join();
  }
  m_threads.clear();
}

void HashServerImp::stop() {
  m_ioContext.stop();
  work();
}

void HashServerImp::startAccept() {
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
