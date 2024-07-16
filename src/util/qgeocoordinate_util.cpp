#include "archimedes.h"

#include "util/qgeocoordinate_util.h"

#include "util/fractional_int.h"

namespace archimedes::qgeocoordinate {

void simplify(QGeoCoordinate &qgeocoordinate)
{
	qgeocoordinate.setLongitude(centesimal_int(qgeocoordinate.longitude()).to_double());
	qgeocoordinate.setLatitude(centesimal_int(qgeocoordinate.latitude()).to_double());
}

}
