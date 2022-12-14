#include "archimedes.h"

#include "util/number_util.h"

#include "util/fractional_int.h"

#include <boost/test/unit_test.hpp>

using namespace archimedes;

template <typename number_type>
static void check_sign()
{
    BOOST_CHECK(number::sign(number_type(6)) == 1);
    BOOST_CHECK(number::sign(number_type(-5)) == -1);
    BOOST_CHECK(number::sign(number_type(0)) == 0);
}

BOOST_AUTO_TEST_CASE(sign_test)
{
    check_sign<int8_t>();
    check_sign<int16_t>();
    check_sign<int32_t>();
    check_sign<int64_t>();
    check_sign<float>();
    check_sign<double>();
    check_sign<fractional_int<1>>();
    check_sign<fractional_int<2>>();
    check_sign<fractional_int<3>>();
    check_sign<fractional_int<4>>();
}

template <typename number_type>
static void check_sqrt()
{
    BOOST_CHECK(number::sqrt<number_type>(1) == 1);
    BOOST_CHECK(number::sqrt<number_type>(2) == 1);
    BOOST_CHECK(number::sqrt<number_type>(3) == 1);
    BOOST_CHECK(number::sqrt<number_type>(4) == 2);
    BOOST_CHECK(number::sqrt<number_type>(5) == 2);
    BOOST_CHECK(number::sqrt<number_type>(6) == 2);
    BOOST_CHECK(number::sqrt<number_type>(7) == 2);
    BOOST_CHECK(number::sqrt<number_type>(8) == 2);
    BOOST_CHECK(number::sqrt<number_type>(9) == 3);
    BOOST_CHECK(number::sqrt<number_type>(10) == 3);
    BOOST_CHECK(number::sqrt<number_type>(18) == 4);
    BOOST_CHECK(number::sqrt<number_type>(27) == 5);
    BOOST_CHECK(number::sqrt<number_type>(32) == 5);
    BOOST_CHECK(number::sqrt<number_type>(64) == 8);
    BOOST_CHECK(number::sqrt<number_type>(100) == 10);
    BOOST_CHECK(number::sqrt<number_type>(101) == 10);

    if constexpr (static_cast<uint64_t>(std::numeric_limits<number_type>::max()) >= static_cast<uint64_t>(std::numeric_limits<int16_t>::max())) {
        BOOST_CHECK(number::sqrt<number_type>(317) == 17);
        BOOST_CHECK(number::sqrt<number_type>(700) == 26);
        BOOST_CHECK(number::sqrt<number_type>(778) == 27);
        BOOST_CHECK(number::sqrt<number_type>(1000) == 31);
        BOOST_CHECK(number::sqrt<number_type>(1024) == 32);
        BOOST_CHECK(number::sqrt<number_type>(1030) == 32);
        BOOST_CHECK(number::sqrt<number_type>(2900) == 53);
    }

    if constexpr (static_cast<uint64_t>(std::numeric_limits<number_type>::max()) >= static_cast<uint64_t>(std::numeric_limits<int32_t>::max())) {
        BOOST_CHECK(number::sqrt<number_type>(75900) == 275);
    }

    if constexpr (static_cast<uint64_t>(std::numeric_limits<number_type>::max()) >= static_cast<uint64_t>(std::numeric_limits<int64_t>::max())) {
        BOOST_CHECK(number::sqrt<number_type>(14400000000) == 120000);
    }
}

BOOST_AUTO_TEST_CASE(isqrt_test_int8)
{
    check_sqrt<int8_t>();
}

BOOST_AUTO_TEST_CASE(isqrt_test_uint8)
{
    check_sqrt<uint8_t>();
}

BOOST_AUTO_TEST_CASE(isqrt_test_int16)
{
    check_sqrt<int16_t>();
}

BOOST_AUTO_TEST_CASE(isqrt_test_uint16)
{
    check_sqrt<uint16_t>();
}

BOOST_AUTO_TEST_CASE(isqrt_test_int32)
{
    check_sqrt<int32_t>();
}

BOOST_AUTO_TEST_CASE(isqrt_test_uint32)
{
    check_sqrt<uint32_t>();
}

BOOST_AUTO_TEST_CASE(isqrt_test_int64)
{
    check_sqrt<int64_t>();
}

BOOST_AUTO_TEST_CASE(isqrt_test_uint64)
{
    check_sqrt<uint64_t>();
}

template <typename number_type>
static void check_cbrt()
{
    BOOST_CHECK(number::cbrt<number_type>(1) == 1);
    BOOST_CHECK(number::cbrt<number_type>(2) == 1);
    BOOST_CHECK(number::cbrt<number_type>(3) == 1);
    BOOST_CHECK(number::cbrt<number_type>(4) == 1);
    BOOST_CHECK(number::cbrt<number_type>(5) == 1);
    BOOST_CHECK(number::cbrt<number_type>(6) == 1);
    BOOST_CHECK(number::cbrt<number_type>(7) == 1);
    BOOST_CHECK(number::cbrt<number_type>(8) == 2);
    BOOST_CHECK(number::cbrt<number_type>(9) == 2);
    BOOST_CHECK(number::cbrt<number_type>(10) == 2);
    BOOST_CHECK(number::cbrt<number_type>(18) == 2);
    BOOST_CHECK(number::cbrt<number_type>(27) == 3);
    BOOST_CHECK(number::cbrt<number_type>(32) == 3);
    BOOST_CHECK(number::cbrt<number_type>(64) == 4);
    BOOST_CHECK(number::cbrt<number_type>(100) == 4);
    BOOST_CHECK(number::cbrt<number_type>(101) == 4);

    if constexpr (static_cast<uint64_t>(std::numeric_limits<number_type>::max()) >= static_cast<uint64_t>(std::numeric_limits<int16_t>::max())) {
        BOOST_CHECK(number::cbrt<number_type>(317) == 6);
        BOOST_CHECK(number::cbrt<number_type>(700) == 8);
        BOOST_CHECK(number::cbrt<number_type>(778) == 9);
        BOOST_CHECK(number::cbrt<number_type>(1000) == 10);
        BOOST_CHECK(number::cbrt<number_type>(1024) == 10);
        BOOST_CHECK(number::cbrt<number_type>(1030) == 10);
        BOOST_CHECK(number::cbrt<number_type>(2900) == 14);
    }

    if constexpr (static_cast<uint64_t>(std::numeric_limits<number_type>::max()) >= static_cast<uint64_t>(std::numeric_limits<int32_t>::max())) {
        BOOST_CHECK(number::cbrt<number_type>(75900) == 42);
    }

    if constexpr (static_cast<uint64_t>(std::numeric_limits<number_type>::max()) >= static_cast<uint64_t>(std::numeric_limits<int64_t>::max())) {
        BOOST_CHECK(number::cbrt<number_type>(14400000000) == 2432);
    }
}

BOOST_AUTO_TEST_CASE(icbrt_test_int8)
{
    check_cbrt<int8_t>();
}

BOOST_AUTO_TEST_CASE(icbrt_test_uint8)
{
    check_cbrt<uint8_t>();
}

BOOST_AUTO_TEST_CASE(icbrt_test_int16)
{
    check_cbrt<int16_t>();
}

BOOST_AUTO_TEST_CASE(icbrt_test_uint16)
{
    check_cbrt<uint16_t>();
}

BOOST_AUTO_TEST_CASE(icbrt_test_int32)
{
    check_cbrt<int32_t>();
}

BOOST_AUTO_TEST_CASE(icbrt_test_uint32)
{
    check_cbrt<uint32_t>();
}

BOOST_AUTO_TEST_CASE(icbrt_test_int64)
{
    check_cbrt<int64_t>();
}

BOOST_AUTO_TEST_CASE(icbrt_test_uint64)
{
    check_cbrt<uint64_t>();
}
