#include "archimedes.h"

#include "util/geopolygon_util.h"

#include "util/qgeocoordinate_util.h"

namespace archimedes::geopolygon {

void simplify(QGeoPolygon &geopolygon)
{
	const QList<QGeoCoordinate> geocoordinates = geopolygon.perimeter();
	for (int i = 0; i < geocoordinates.size(); ++i) {
		const QGeoCoordinate &qgeocoordinate = geocoordinates.at(i);
		geopolygon.replaceCoordinate(i, qgeocoordinate::simplified(qgeocoordinate));
	}

	std::vector<QList<QGeoCoordinate>> hole_paths;
	for (int i = 0; i < geopolygon.holesCount(); ++i) {
		hole_paths.push_back(geopolygon.holePath(i));
	}

	while (geopolygon.holesCount() > 0) {
		geopolygon.removeHole(0);
	}

	for (QList<QGeoCoordinate> &hole_path : hole_paths) {
		for (QGeoCoordinate &qgeocoordinate : hole_path) {
			qgeocoordinate::simplify(qgeocoordinate);
		}

		geopolygon.addHole(hole_path);
	}
}

}
