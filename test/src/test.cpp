#define BOOST_TEST_MODULE hash_server_tests
//#define BOOST_TEST_DYN_LINK 1

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <stdio.h>
#include <picosha2.h>
#include <HashServerImp.h>

#ifdef _WIN32
  const std::string delimiter = "\r\n";
#elif __linux__
  const std::string delimiter = "\n";
#endif
const std::size_t shift = delimiter.length();

std::string getRandomString(const size_t &size) {
  static const char alphanum[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";

  std::string result = "";
  for (size_t i = 0; i < size; ++i) {
    result.push_back(alphanum[rand() % (sizeof(alphanum) - 1)]);
  }

  return result;
}

class HashClient {
private:
  tcp::resolver m_resolver;
  tcp::socket m_socket;
  tcp::resolver::results_type m_endpoints;
  boost::asio::streambuf m_buf;

public:
  HashClient(boost::asio::io_context &ioContext, const int &port)
    : m_resolver(ioContext), m_socket(ioContext) {
    boost::system::error_code ec;
    m_endpoints = m_resolver.resolve(
      boost::asio::ip::tcp::v4(),
      "127.0.0.1",
      std::to_string(port),
      ec
    );

    boost::asio::connect(m_socket, m_endpoints, ec);
  }

  void send(std::string source, const bool &addEOL = true) {
    boost::system::error_code error;
    if (addEOL) {
      source += delimiter;
    }
    boost::asio::write(m_socket, boost::asio::buffer(source), error);
  }

  std::string recieve() {
    boost::system::error_code ec;
    std::size_t received = boost::asio::read_until(m_socket, m_buf, '\n', ec);
    if (ec == boost::asio::error::eof) { // connection closed
      return "";
    }
    const std::size_t payloadLen = received - shift;

    std::istream is(&m_buf);
    std::string hexStr;
    std::getline(is, hexStr);
    if (shift > 1) {
      hexStr.erase(hexStr.end() - (shift - 1));
    }
    m_buf.consume(received);

    return hexStr;
  }

  std::string retrieve(std::string source, const bool &addEOL = true) {
    send(source, addEOL);
    return recieve();
  }

};

BOOST_AUTO_TEST_SUITE(hash_server_tests)



  BOOST_AUTO_TEST_CASE(PlusTest) {
    BOOST_CHECK_EQUAL(1 + 2, 3);
  }

  BOOST_AUTO_TEST_CASE(MinusTest) {
    BOOST_CHECK_EQUAL(2 - 1, 1);
  }

  const std::map<std::string, std::string> predefinedHashes = {
    {"111", "f6e0a1e2ac41945a9aa7ff8a8aaa0cebc12a3bcc981a929ad5cf810a090e11ae"},
    {"222", "9b871512327c09ce91dd649b3f96a63b7408ef267c8cc5710114e629730cb61f"},
    {"333", "556d7dc3a115356350f1f9910b1af1ab0e312d4b3e4fc788d2da63668f36d017"}
  };

  BOOST_AUTO_TEST_CASE(HashPredefinedTest) {
    for (auto iHash : predefinedHashes) {
      const std::string calculated = picosha2::hash256_hex_string(iHash.first);
      const std::string predefined = iHash.second;
      BOOST_CHECK_EQUAL(calculated, predefined);
    }
  }

  BOOST_AUTO_TEST_CASE(HashRandomTest) {
    for (int i = 0; i < 10; ++i) {
      const std::string source = getRandomString(100);
      const std::string calculated = picosha2::hash256_hex_string(source);
      const std::string hash = picosha2::hash256_hex_string(source);
      BOOST_CHECK_EQUAL(calculated, hash); // dzen of equality
    }
  }

  BOOST_AUTO_TEST_CASE(ServerPredefinedTest) {
    const int serverPort = 6767;
    HashServer::Pointer server = HashServer::create(serverPort);
    server->start(1);

    boost::asio::io_context ioContext;
    HashClient client(ioContext, serverPort);
    {
      std::string longSource = "";
      for (auto iHash : predefinedHashes) {
        longSource += iHash.first + delimiter;
      }
      client.send(longSource, false);

      for (auto iHash : predefinedHashes) {
        std::string recived = client.recieve();
        BOOST_CHECK_EQUAL(recived, iHash.second);
      }
    }


    for (auto iHash : predefinedHashes) {
      const std::string retrived = client.retrieve(iHash.first);
      const std::string predefined = iHash.second;

      BOOST_CHECK_EQUAL(retrived, predefined);
    }

    /*int i;
    std::cout << "Enter any int to exit: " << std::endl;
    std::cin >> i;*/

    ioContext.stop();
    server->stop();
  }

  BOOST_AUTO_TEST_CASE(ServerRandomTest) {
    const int serverPort = 6767;
    HashServer::Pointer server = HashServer::create(serverPort);
    server->start(4);

    boost::asio::io_context ioContext;
    HashClient client(ioContext, serverPort);
    for (int i = 0; i < 10; ++i) {
      const std::string source = getRandomString(100);
      const std::string calculated = picosha2::hash256_hex_string(source);
      const std::string retrived = client.retrieve(source);
      BOOST_CHECK_EQUAL(calculated, retrived);
    }

    ioContext.stop();
    server->stop();
  }

  BOOST_AUTO_TEST_CASE(ServerStressTest) {
    const int serverPort = 6767;
    HashServer::Pointer server = HashServer::create(serverPort);

    unsigned int threadsCount = std::thread::hardware_concurrency();
    threadsCount = threadsCount ? threadsCount : 1;
    server->start(threadsCount);

    std::vector<std::shared_ptr<std::thread>> threads;
    boost::asio::io_context ioContext;
    for (int k = 0; k < 10; ++k) {
      threads.push_back(
        std::shared_ptr<std::thread>(new std::thread(
          [&ioContext, serverPort]() mutable -> void {
            HashClient client(ioContext, serverPort);

            std::vector<std::pair<std::string, std::string>> hashes = {};
            static const size_t size = 1024 * 256 * 10;
            for (int i = 0; i < 10; ++i) {
              const std::string source = getRandomString(size);
              const std::string calculated = picosha2::hash256_hex_string(source);
              hashes.push_back(std::make_pair(source, calculated));
            }

            for (auto iHash : hashes) {
              const std::string retrived = client.retrieve(iHash.first);
              BOOST_CHECK_EQUAL(iHash.second, retrived);
            }
          }
        ))
      );
    }

    for (auto iThread : threads) {
      iThread->join();
    }

    ioContext.stop();
    server->stop();
  }

BOOST_AUTO_TEST_SUITE_END()
