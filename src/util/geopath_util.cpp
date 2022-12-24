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

	std::optional<QPoint> previous_pixel_pos;
	geocoordinate start_geocoordinate;
	geocoordinate end_geocoordinate;

	for (const QGeoCoordinate &qgeocoordinate : geopath.path()) {
		const archimedes::geocoordinate geocoordinate(qgeocoordinate);
		const QPoint pixel_pos = map_projection->geocoordinate_to_point(geocoordinate, georectangle, image.size(), x_offset);

		if (pixel_pos == previous_pixel_pos) {
			end_geocoordinate = geocoordinate;
			continue;
		}

		//only write to the image if the position is actually in it
		if (previous_pixel_pos.has_value() && image_rect.contains(previous_pixel_pos.value())) {
			uint8_t direction_flags = 0;

			const QColor old_pixel_color = image.pixelColor(previous_pixel_pos.value());
			if (old_pixel_color.alpha() != 0) {
				direction_flags = static_cast<uint8_t>(old_pixel_color.blue());
			}

			uint8_t new_direction_flags = direction_flags;

			const archimedes::geocoordinate tile_center_geocoordinate = map_projection->point_to_geocoordinate(previous_pixel_pos.value(), georectangle, image.size(), x_offset);

			log::log_error("Start Geocoordinate: (" + start_geocoordinate.get_longitude().to_string() + ", " + start_geocoordinate.get_latitude().to_string() + "), End Geocoordinate: (" + end_geocoordinate.get_longitude().to_string() + ", " + end_geocoordinate.get_latitude().to_string() + ")");

			const QPoint start_reference_pos = map_projection->geocoordinate_to_point(start_geocoordinate, georectangle, image.size() * 100, x_offset * 100);
			const QPoint end_reference_pos = map_projection->geocoordinate_to_point(end_geocoordinate, georectangle, image.size() * 100, x_offset * 100);
			const int longitude_distance = std::abs(start_reference_pos.x() - end_reference_pos.x());
			const int latitude_distance = std::abs(start_reference_pos.y() - end_reference_pos.y());

			if (longitude_distance >= 50) {
				if (start_geocoordinate.get_latitude() <= tile_center_geocoordinate.get_latitude()) {
					new_direction_flags |= direction_flag::south;
				} else {
					new_direction_flags |= direction_flag::north;
				}
			}

			if (latitude_distance >= 50) {
				if (start_geocoordinate.get_longitude() >= tile_center_geocoordinate.get_longitude()) {
					new_direction_flags |= direction_flag::east;
				} else {
					new_direction_flags |= direction_flag::west;
				}
			}

			if (new_direction_flags != direction_flags) {
				geoshape::write_pixel_to_image(previous_pixel_pos.value(), QColor(0, 0, new_direction_flags), image);
			}
		}

		previous_pixel_pos = pixel_pos;
		start_geocoordinate = geocoordinate;
	}
}

}
