#include "archimedes.h"

#include "util/geoshape_util.h"

#include "map/map_projection.h"
#include "util/geocoordinate.h"
#include "util/geocircle_util.h"
#include "util/geopath_util.h"
#include "util/georectangle.h"
#include "util/point_util.h"

namespace archimedes::geoshape {

void write_to_image(const QGeoShape &geoshape, QImage &image, const QColor &color, const georectangle &georectangle, const map_projection *map_projection, const std::string &image_checkpoint_save_filename)
{
	const QGeoRectangle qgeorectangle = georectangle.to_qgeorectangle();
	QGeoRectangle bounding_qgeorectangle = geoshape.boundingGeoRectangle();

	if (!bounding_qgeorectangle.intersects(qgeorectangle)) {
		return;
	}

	switch (geoshape.type()) {
		case QGeoShape::PathType: {
			const QGeoPath &geopath = static_cast<const QGeoPath &>(geoshape);
			geopath::write_to_image(geopath, image, color, georectangle, map_projection);

			//if the geopath's width is 0, there is nothing further to do here, but otherwise, use the normal method of geoshape writing as well
			if (geopath.width() == 0) {
				return;
			}

			//increase the bounding rectangle of geopaths slightly, as otherwise a part of the path's width is cut off
			QGeoCoordinate bottom_left = bounding_qgeorectangle.bottomLeft();
			QGeoCoordinate top_right = bounding_qgeorectangle.topRight();
			bottom_left.setLatitude(bottom_left.latitude() - 0.1);
			bottom_left.setLongitude(bottom_left.longitude() - 0.1);
			top_right.setLatitude(top_right.latitude() + 0.1);
			top_right.setLongitude(top_right.longitude() + 0.1);
			bounding_qgeorectangle.setBottomLeft(bottom_left);
			bounding_qgeorectangle.setTopRight(top_right);
			break;
		}
		case QGeoShape::CircleType: {
			const QGeoCircle &geocircle = static_cast<const QGeoCircle &>(geoshape);
			geocircle::write_to_image(geocircle, image, color, georectangle, map_projection);

			if (geocircle.radius() == -1) {
				return;
			}
			break;
		}
		default:
			break;
	}

	const archimedes::georectangle bounding_georectangle = georectangle::from_qgeorectangle(bounding_qgeorectangle);

	longitude start_lon = bounding_georectangle.get_min_longitude() - 1;
	if (start_lon < geocoordinate::min_longitude) {
		start_lon = geocoordinate::min_longitude;
	}

	longitude end_lon = bounding_georectangle.get_max_longitude() + 1;
	if (end_lon > geocoordinate::max_longitude) {
		end_lon = geocoordinate::max_longitude;
	}

	latitude start_lat = bounding_georectangle.get_max_latitude() + 1;
	if (start_lat > geocoordinate::max_latitude) {
		start_lat = geocoordinate::max_latitude;
	}

	latitude end_lat = bounding_georectangle.get_min_latitude() - 1;
	if (end_lat < geocoordinate::min_latitude) {
		end_lat = geocoordinate::min_latitude;
	}

	const geocoordinate start_geocoordinate(start_lon, start_lat);
	const geocoordinate end_geocoordinate(end_lon, end_lat);

	QPoint start_pos = map_projection->geocoordinate_to_point(start_geocoordinate, georectangle, image.size());
	if (start_pos.x() < 0) {
		start_pos.setX(0);
	}
	if (start_pos.y() < 0) {
		start_pos.setY(0);
	}

	QPoint end_pos = map_projection->geocoordinate_to_point(end_geocoordinate, georectangle, image.size());
	if (end_pos.x() > (image.width() - 1)) {
		end_pos.setX(image.width() - 1);
	}
	if (end_pos.y() > (image.height() - 1)) {
		end_pos.setY(image.height() - 1);
	}

	int pixel_checkpoint_count = 0;
	static constexpr int pixel_checkpoint_threshold = 32 * 32;

	for (int x = start_pos.x(); x <= end_pos.x(); ++x) {
		for (int y = start_pos.y(); y <= end_pos.y(); ++y) {
			const QPoint pixel_pos(x, y);

			if (image.pixelColor(pixel_pos).alpha() != 0) {
				continue; //ignore already-written pixels
			}

			const geocoordinate geocoordinate = map_projection->point_to_geocoordinate(pixel_pos, georectangle, image.size());
			const QGeoCoordinate qgeocoordinate = geocoordinate.to_qgeocoordinate();

			if (!geoshape.contains(qgeocoordinate)) {
				continue;
			}

			image.setPixelColor(pixel_pos, color);
			++pixel_checkpoint_count;

			if (pixel_checkpoint_count >= pixel_checkpoint_threshold) {
				image.save(QString::fromStdString(image_checkpoint_save_filename));
				pixel_checkpoint_count = 0;
			}
		}
	}
}

}