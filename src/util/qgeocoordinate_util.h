#pragma once

namespace archimedes::qgeocoordinate {

extern void simplify(QGeoCoordinate &geocoordinate);

inline QGeoCoordinate simplified(const QGeoCoordinate &geocoordinate)
{
	QGeoCoordinate simplified_geocoordinate = geocoordinate;
	qgeocoordinate::simplify(simplified_geocoordinate);
	return simplified_geocoordinate;
}

}
