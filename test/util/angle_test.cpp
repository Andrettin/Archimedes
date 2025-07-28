#include "archimedes.h"

#include "util/angle_util.h"

#include "util/decimillesimal_int.h"

#include <boost/test/unit_test.hpp>

using namespace archimedes;

BOOST_AUTO_TEST_CASE(degrees_to_radians_test)
{
    BOOST_CHECK(angle::degrees_to_radians(angle::number_type(180)) == angle::pi);
    BOOST_CHECK(angle::degrees_to_radians(angle::number_type(90)) == angle::number_type("1.5708"));
}

BOOST_AUTO_TEST_CASE(radians_to_degrees_test)
{
    BOOST_CHECK(angle::radians_to_degrees(angle::pi) == 180);
    BOOST_CHECK(angle::radians_to_degrees(angle::number_type("1.5708")) == 90);
}

BOOST_AUTO_TEST_CASE(gudermannian_test)
{
    const angle::number_type y_max = angle::gudermannian_inverse(angle::number_type(85));
    const angle::number_type y_min = angle::gudermannian_inverse(angle::number_type(-85));
    const int height = 1588;
    
    const angle::number_type original_y(615);
    const angle::number_type degrees = angle::gudermannian(y_max - ((original_y / height) * (y_max - y_min)));
    BOOST_CHECK(degrees == angle::number_type("37.4770"));
}
