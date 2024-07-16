#pragma once

namespace archimedes::geopolygon {

extern void simplify(QGeoPolygon &geopolygon);

inline QPolygonF to_polygon(const QGeoPolygon &geopolygon)
{
	QList<QPointF> points;

	for (const QGeoCoordinate &geocoordinate : geopolygon.perimeter()) {
		points.push_back(QPointF(geocoordinate.longitude(), geocoordinate.latitude()));
	}

	return QPolygonF(points);
}

inline std::unique_ptr<QGeoPolygon> from_polygon(const QPolygonF &polygon)
{
	QList<QGeoCoordinate> geocoordinates;

	for (const QPointF &point : polygon) {
		geocoordinates.push_back(QGeoCoordinate(point.y(), point.x()));
	}

	return std::make_unique<QGeoPolygon>(geocoordinates);
}

}
