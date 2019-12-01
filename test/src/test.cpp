#define BOOST_TEST_MODULE hash_server_tests
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <iostream>

BOOST_AUTO_TEST_SUITE(hash_server_tests)

BOOST_AUTO_TEST_CASE(PlusTest) {
    BOOST_CHECK_EQUAL(1 + 2, 3);
}

BOOST_AUTO_TEST_CASE(MinusTest) {
    BOOST_CHECK_EQUAL(2 - 1, 1);
}

BOOST_AUTO_TEST_SUITE_END()
