#include "archimedes.h"

#include "util/time_util.h"

#include "util/fractional_int.h"

#include <boost/test/unit_test.hpp>

using namespace archimedes;

BOOST_AUTO_TEST_CASE(time_to_number_test_1)
{
    const QTime time(6, 28, 45, 710);
    const decimillesimal_int n = time::to_number(time);

    BOOST_CHECK(n == decimillesimal_int("6.4792"));
}

BOOST_AUTO_TEST_CASE(time_to_number_test_2)
{
    const QTime time(19, 40, 5, 791);
    const decimillesimal_int n = time::to_number(time);

    BOOST_CHECK(n == decimillesimal_int("19.6682"));
}
