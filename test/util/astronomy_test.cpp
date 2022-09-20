#include "archimedes.h"

#include "util/astronomy_util.h"

#include "util/fractional_int.h"

#include <boost/test/unit_test.hpp>

using namespace archimedes;

BOOST_AUTO_TEST_CASE(pc_to_ly_test)
{
    const centesimal_int pc("76.28");
    const centesimal_int ly = astronomy::pc_to_ly(pc);

    BOOST_CHECK(ly == centesimal_int("248.67"));
}

BOOST_AUTO_TEST_CASE(ly_to_pc_test)
{
    const centesimal_int ly(382);
    const centesimal_int pc = astronomy::ly_to_pc(ly);

    BOOST_CHECK(pc == centesimal_int("117.17"));
}

BOOST_AUTO_TEST_CASE(au_to_gm_test)
{
    const centesimal_int au(23);
    const int gm = astronomy::au_to_gm(au);

    BOOST_CHECK(gm == 3450);
}

BOOST_AUTO_TEST_CASE(gm_to_au_test)
{
    const int gm = 778;
    const centesimal_int au = astronomy::gm_to_au(gm);

    BOOST_CHECK(au == centesimal_int("5.18"));
}

BOOST_AUTO_TEST_CASE(ra_to_lon_test_1)
{
    const decimillesimal_int ra("6.4792");

    const decimillesimal_int lon = astronomy::ra_to_lon(ra);

    BOOST_CHECK(lon == decimillesimal_int("97.188"));
}

BOOST_AUTO_TEST_CASE(ra_to_lon_test_2)
{
    const decimillesimal_int ra("19.6682");

    const decimillesimal_int lon = astronomy::ra_to_lon(ra);

    BOOST_CHECK(lon == decimillesimal_int("-64.977"));
}

BOOST_AUTO_TEST_CASE(jovian_mass_to_zg_test)
{
    uint64_t zg = astronomy::jovian_mass_to_zg(centesimal_int(1));
    BOOST_CHECK(zg == astronomy::zg_per_jovian_mass);

    zg = astronomy::jovian_mass_to_zg(centesimal_int("0.46"));
    BOOST_CHECK(zg == 873139800);

    zg = astronomy::jovian_mass_to_zg(centesimal_int("1.19"));
    BOOST_CHECK(zg == 2258774700);

    zg = astronomy::jovian_mass_to_zg(centesimal_int("1.5"));
    BOOST_CHECK(zg == 2847195000);
}
