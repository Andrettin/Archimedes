#include "archimedes.h"

#include "util/geopath_util.h"

#include "map/direction.h"
#include "map/map_projection.h"
#include "util/geocoordinate.h"
#include "util/georectangle.h"
#include "util/geoshape_util.h"
#include "util/point_util.h"

namespace archimedes::geopath {

void write_to_image(const QGeoPath &geopath, QImage &image, const QColor &color, const georectangle &georectangle, const map_projection *map_projection, const int x_offset)
{
	QPoint previous_pixel_pos(-1, -1);
	for (const QGeoCoordinate &geocoordinate : geopath.path()) {
		const QPoint pixel_pos = map_projection->geocoordinate_to_point(archimedes::geocoordinate(geocoordinate), georectangle, image.size(), x_offset);

		if (pixel_pos.x() >= 0 && pixel_pos.y() >= 0 && pixel_pos.x() < image.width() && pixel_pos.y() < image.height()) {
			//only write to the image if the position is actually in it, but take the position into account either way for the purpose of getting the previous pixel pos, so that map templates fit together well
			geoshape::write_pixel_to_image(pixel_pos, color, image);
		}

		if (previous_pixel_pos != QPoint(-1, -1) && !point::is_cardinally_adjacent_to(pixel_pos, previous_pixel_pos)) {
			const std::vector<QPoint> straight_pixel_path = point::get_straight_path_to(previous_pixel_pos, pixel_pos);

			for (const QPoint &straight_path_pos : straight_pixel_path) {
				if (straight_path_pos.x() >= 0 && straight_path_pos.y() >= 0 && straight_path_pos.x() < image.width() && straight_path_pos.y() < image.height()) {
					geoshape::write_pixel_to_image(straight_path_pos, color, image);
				}
			}
		}

		previous_pixel_pos = pixel_pos;
	}
}

void write_edges_to_image(const QGeoPath &geopath, QImage &image, const georectangle &georectangle, const map_projection *map_projection, const int x_offset)
{
	const QRect image_rect = image.rect();

	for (const QGeoCoordinate &qgeocoordinate : geopath.path()) {
		const archimedes::geocoordinate geocoordinate(qgeocoordinate);
		const QPoint pixel_pos = map_projection->geocoordinate_to_point(geocoordinate, georectangle, image.size(), x_offset);

		if (!image_rect.contains(pixel_pos)) {
			//only write to the image if the position is actually in it
			continue;
		}

		uint8_t direction_flags = static_cast<uint8_t>(image.pixelColor(pixel_pos).blue());

		uint8_t new_direction_flags = direction_flags;

		const archimedes::geocoordinate tile_center_geocoordinate = map_projection->point_to_geocoordinate(pixel_pos, georectangle, image.size(), x_offset);

		const QPoint reference_pos = map_projection->geocoordinate_to_point(geocoordinate, georectangle, image.size() * 100, x_offset * 100);
		const QPoint tile_center_reference_pos = map_projection->geocoordinate_to_point(tile_center_geocoordinate, georectangle, image.size() * 100, x_offset * 100);
		const int longitude_distance = std::abs(reference_pos.x() - tile_center_reference_pos.x());
		const int latitude_distance = std::abs(reference_pos.y() - tile_center_reference_pos.y());

		if (longitude_distance >= 50) {
			if (geocoordinate.get_latitude() <= tile_center_geocoordinate.get_latitude()) {
				new_direction_flags |= direction_flag::south;
			} else {
				new_direction_flags |= direction_flag::north;
			}
		}

		if (latitude_distance >= 50) {
			if (geocoordinate.get_longitude() >= tile_center_geocoordinate.get_longitude()) {
				new_direction_flags |= direction_flag::east;
			} else {
				new_direction_flags |= direction_flag::west;
			}
		}

		if (new_direction_flags == direction_flags) {
			continue;
		}

		geoshape::write_pixel_to_image(pixel_pos, QColor(0, 0, new_direction_flags), image);
	}
}

}
