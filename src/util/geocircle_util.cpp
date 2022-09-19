#include "archimedes.h"

#include "util/geocircle_util.h"

#include "map/map_projection.h"
#include "util/geocoordinate.h"
#include "util/geoshape_util.h"

namespace archimedes::geocircle {

void write_to_image(const QGeoCircle &geocircle, QImage &image, const QColor &color, const georectangle &georectangle, const map_projection *map_projection)
{
	const QGeoCoordinate geocoordinate = geocircle.center();

	const QPoint base_pixel_pos = map_projection->geocoordinate_to_point(archimedes::geocoordinate(geocoordinate), georectangle, image.size());

	//write a 2x2 block so that the point can be expanded from in terrain generation, and so that it won't be removed if the terrain type does not allow single tiles
	for (int x_offset = 0; x_offset <= 1; ++x_offset) {
		for (int y_offset = 0; y_offset <= 1; ++y_offset) {
			const QPoint pixel_pos = base_pixel_pos + QPoint(x_offset, y_offset);

			if (pixel_pos.x() >= 0 && pixel_pos.y() >= 0 && pixel_pos.x() < image.width() && pixel_pos.y() < image.height()) {
				geoshape::write_pixel_to_image(pixel_pos, color, image);
			}
		}
	}
}

}
