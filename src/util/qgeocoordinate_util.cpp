#include "archimedes.h"

#include "util/qgeocoordinate_util.h"

#include "util/centesimal_int.h"

namespace archimedes::qgeocoordinate {

void simplify(QGeoCoordinate &geocoordinate)
{
	geocoordinate.setLongitude(centesimal_int(geocoordinate.longitude()).to_double());
	geocoordinate.setLatitude(centesimal_int(geocoordinate.latitude()).to_double());
}

}
