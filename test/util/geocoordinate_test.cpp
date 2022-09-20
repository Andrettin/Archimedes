#include "archimedes.h"

#include "util/geocoordinate.h"

#include "map/equirectangular_map_projection.h"
#include "map/mercator_map_projection.h"
#include "util/georectangle.h"
#include "util/point_util.h"

#include <boost/test/unit_test.hpp>

using namespace archimedes;

BOOST_AUTO_TEST_CASE(geocoordinate_to_circle_edge_point_test_1)
{
    const geocoordinate geocoordinate(-35, 0);
    const QPoint direction_pos = geocoordinate.to_circle_edge_point();

    const QPoint expected_result(-10000, 0);
    BOOST_TEST((direction_pos == expected_result), "direction_pos " + point::to_string(direction_pos) + " does not equal " + point::to_string(expected_result));
}

BOOST_AUTO_TEST_CASE(geocoordinate_to_circle_edge_point_test_2)
{
    const geocoordinate geocoordinate(-90, 45);
    const QPoint direction_pos = geocoordinate.to_circle_edge_point();

    const QPoint expected_result(-7071, -7071);
    BOOST_TEST((direction_pos == expected_result), "direction_pos " + point::to_string(direction_pos) + " does not equal " + point::to_string(expected_result));
}

static void check_point_to_geocoordinate(const map_projection *map_projection)
{
    const QSize size(384, 192);

    const geocoordinate::number_type min_longitude(-11);
    const geocoordinate::number_type max_longitude(34);
    const geocoordinate::number_type min_latitude(45);
    const geocoordinate::number_type max_latitude(60);

    const georectangle georectangle(geocoordinate(min_longitude, min_latitude), geocoordinate(max_longitude, max_latitude));

    geocoordinate geocoordinate = map_projection->point_to_geocoordinate(QPoint(0, 0), georectangle, size);
    geocoordinate::number_type lon = geocoordinate.get_longitude();
    BOOST_CHECK((lon - min_longitude).abs() <= 1);
    geocoordinate::number_type lat = geocoordinate.get_latitude();
    BOOST_CHECK((lat - max_latitude).abs() <= 1);

    geocoordinate = map_projection->point_to_geocoordinate(QPoint(384, 192), georectangle, size);
    lon = geocoordinate.get_longitude();
    BOOST_CHECK((lon - max_longitude).abs() <= 1);
    lat = geocoordinate.get_latitude();
    BOOST_CHECK((lat - min_latitude).abs() <= 1);
}

BOOST_AUTO_TEST_CASE(equirectangular_point_to_geocoordinate_test)
{
    const map_projection *map_projection = equirectangular_map_projection::get();
    check_point_to_geocoordinate(map_projection);
}

BOOST_AUTO_TEST_CASE(mercator_point_to_geocoordinate_test)
{
    const map_projection *map_projection = mercator_map_projection::get();
    check_point_to_geocoordinate(map_projection);
}

BOOST_AUTO_TEST_CASE(georectangle_conversion_test)
{
    const geocoordinate::number_type min_longitude(-11);
    const geocoordinate::number_type max_longitude(34);
    const geocoordinate::number_type min_latitude(45);
    const geocoordinate::number_type max_latitude(60);

    const georectangle georectangle(geocoordinate(min_longitude, min_latitude), geocoordinate(max_longitude, max_latitude));

    const QGeoRectangle qgeorectangle = georectangle.to_qgeorectangle();
    const archimedes::georectangle converted_georectangle = georectangle::from_qgeorectangle(qgeorectangle);
    BOOST_CHECK(georectangle == converted_georectangle);
}
