#include "archimedes.h"

#include "util/geopolygon_util.h"

#include "util/qgeocoordinate_util.h"

namespace archimedes::geopolygon {

void simplify(QGeoPolygon &geopolygon)
{
	const QList<QGeoCoordinate> geocoordinates = geopolygon.perimeter();
	QList<QGeoCoordinate> simplified_geocoordinates;
	for (int i = 0; i < geocoordinates.size(); ++i) {
		const QGeoCoordinate &geocoordinate = geocoordinates.at(i);
		QGeoCoordinate simplified_geocoordinate = qgeocoordinate::simplified(geocoordinate);
		if (simplified_geocoordinates.empty() || simplified_geocoordinates.back() != simplified_geocoordinate) {
			simplified_geocoordinates.push_back(simplified_geocoordinate);
		}
	}

	std::vector<QList<QGeoCoordinate>> hole_paths;
	for (int i = 0; i < geopolygon.holesCount(); ++i) {
		const QList<QGeoCoordinate> hole_geocoordinates = geopolygon.holePath(i);
		QList<QGeoCoordinate> simplified_hole_geocoordinates;

		for (int j = 0; j < hole_geocoordinates.size(); ++i) {
			const QGeoCoordinate &geocoordinate = hole_geocoordinates.at(j);
			QGeoCoordinate simplified_geocoordinate = qgeocoordinate::simplified(geocoordinate);
			if (simplified_hole_geocoordinates.empty() || simplified_hole_geocoordinates.back() != simplified_geocoordinate) {
				simplified_hole_geocoordinates.push_back(simplified_geocoordinate);
			}
		}

		hole_paths.push_back(simplified_hole_geocoordinates);
	}

	while (geopolygon.holesCount() > 0) {
		geopolygon.removeHole(0);
	}

	for (QList<QGeoCoordinate> &hole_path : hole_paths) {
		geopolygon.addHole(hole_path);
	}
}

}
