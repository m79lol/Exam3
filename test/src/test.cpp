#define BOOST_TEST_MODULE hash_server_tests
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <iostream>

#include <hasher/hasher.h>

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
  
  std::string retrieve(std::string source) {
    boost::system::error_code error;
    source += "\r\n";
    boost::asio::write(m_socket, boost::asio::buffer(source), error);
    
    boost::asio::streambuf buf;
    boost::asio::read_until(m_socket, buf, "\r\n");
    
    std::istream is(&buf);
    std::string hexStr;
    std::getline(is, hexStr);
    hexStr.erase(hexStr.end() - 1);
    
    return hexStr;
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
      const std::string calculated = Hash::calculate(iHash.first);
      const std::string predefined = iHash.second;
      BOOST_CHECK_EQUAL(calculated, predefined);
    }
  }
  
  BOOST_AUTO_TEST_CASE(HashRandomTest) {
    for (int i = 0; i < 10; ++i) {
      const std::string source = getRandomString(100);
      const std::string calculated = Hash::calculate(source);
      const std::string hash = Hash::calculate(source);
      BOOST_CHECK_EQUAL(calculated, hash); // dzen of equality
    }
  }
  
  BOOST_AUTO_TEST_CASE(ServerPredefinedTest) {
    boost::asio::io_context ioContext;
    const int serverPort = 6767;
    
    std::thread thrServer(
      [&ioContext, serverPort]() mutable -> void {
        try {
          HashServer server(ioContext, serverPort);
          ioContext.run();
        }
        catch (std::exception& e) {
          std::cerr << e.what() << std::endl;
        }
      }
    );
  
    HashClient client(ioContext, serverPort);
    for (auto iHash : predefinedHashes) {
      const std::string retrived = client.retrieve(iHash.first);
      const std::string predefined = iHash.second;
      
      BOOST_CHECK_EQUAL(retrived, predefined);
    }

    /*int i;
    std::cout << "Enter any int to exit: " << std::endl;
    std::cin >> i;*/
    
    ioContext.stop();
    thrServer.join();
  }
  
  BOOST_AUTO_TEST_CASE(ServerRandomTest) {
    boost::asio::io_context ioContext;
    const int serverPort = 6767;
    
    std::thread thrServer(
      [&ioContext, serverPort]() mutable -> void {
        try {
          HashServer server(ioContext, serverPort);
          ioContext.run();
        }
        catch (std::exception& e) {
          std::cerr << e.what() << std::endl;
        }
      }
    );
    
    HashClient client(ioContext, serverPort);
    for (int i = 0; i < 10; ++i) {
      const std::string source = getRandomString(100);
      const std::string calculated = Hash::calculate(source);
      const std::string retrived = client.retrieve(source);
      BOOST_CHECK_EQUAL(calculated, retrived);
    }
    
    ioContext.stop();
    thrServer.join();
  }

  BOOST_AUTO_TEST_CASE(ServerStressTest) {
    boost::asio::io_context ioContext;
    const int serverPort = 6767;

    std::thread thrServer(
      [&ioContext, serverPort]() mutable -> void {
      try {
        HashServer server(ioContext, serverPort);
        ioContext.run();
      }
      catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
      }
    }
    );

    std::vector<std::shared_ptr<std::thread>> threads;

    for (int k = 0; k < 10; ++k) {
      threads.push_back(
        std::shared_ptr<std::thread>(new std::thread(
          [&ioContext, serverPort]() mutable -> void {
            HashClient client(ioContext, serverPort);
            for (int i = 0; i < 10; ++i) {

              const size_t size = 1024;

              const std::string source = getRandomString(size);
              const std::string calculated = Hash::calculate(source);
              const std::string retrived = client.retrieve(source);
              BOOST_CHECK_EQUAL(calculated, retrived);
            }
          }
        ))
      );
    }

    for (auto iThread : threads) {
      iThread->join();
    }

    ioContext.stop();
    thrServer.join();
  }

BOOST_AUTO_TEST_SUITE_END()
