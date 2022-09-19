#include "archimedes.h"

#include "util/geopath_util.h"

#include "map/map_projection.h"
#include "util/geocoordinate.h"
#include "util/georectangle.h"
#include "util/geoshape_util.h"
#include "util/point_util.h"

namespace archimedes::geopath {

void write_to_image(const QGeoPath &geopath, QImage &image, const QColor &color, const georectangle &georectangle, const map_projection *map_projection)
{
	QPoint previous_pixel_pos(-1, -1);
	for (const QGeoCoordinate &geocoordinate : geopath.path()) {
		const QPoint pixel_pos = map_projection->geocoordinate_to_point(archimedes::geocoordinate(geocoordinate), georectangle, image.size());

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

}
