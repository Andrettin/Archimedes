#pragma once

#include "util/color_container.h"

namespace archimedes {
	class georectangle;
	class map_projection;
}

namespace archimedes::geoshape {

extern void write_image(const std::filesystem::path &filepath, color_map<std::vector<std::unique_ptr<QGeoShape>>> &geodata_map, const georectangle &georectangle, const QSize &image_size, const map_projection *map_projection, const QImage &base_image, const int geocoordinate_x_offset);

extern void write_to_image(QImage &image, color_map<std::vector<std::unique_ptr<QGeoShape>>> &geodata_map, const georectangle &georectangle, const map_projection *map_projection, const int geocoordinate_x_offset);

extern void write_to_image(const QGeoShape &geoshape, QImage &image, const QColor &color, const georectangle &georectangle, const map_projection *map_projection, const std::filesystem::path &image_checkpoint_save_filepath = "", const int geocoordinate_x_offset = 0);

inline void write_pixel_to_image(const QPoint &pixel_pos, const QColor &color, QImage &image)
{
	if (image.pixelColor(pixel_pos).alpha() != 0) {
		return; //ignore already-written pixels
	}

	image.setPixelColor(pixel_pos, color);
}

}
