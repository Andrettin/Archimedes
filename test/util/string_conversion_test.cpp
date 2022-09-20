#include "archimedes.h"

#include "util/string_conversion_util.h"

#include <boost/test/unit_test.hpp>

using namespace archimedes;

BOOST_AUTO_TEST_CASE(string_to_time_test_1)
{
    const QTime time = string::to_time("6.28.45.710");

    BOOST_CHECK(time.hour() == 6);
    BOOST_CHECK(time.minute() == 28);
    BOOST_CHECK(time.second() == 45);
    BOOST_CHECK(time.msec() == 710);
}

BOOST_AUTO_TEST_CASE(string_to_time_test_2)
{
    const QTime time = string::to_time("19.40.5.791");

    BOOST_CHECK(time.hour() == 19);
    BOOST_CHECK(time.minute() == 40);
    BOOST_CHECK(time.second() == 5);
    BOOST_CHECK(time.msec() == 791);
}
