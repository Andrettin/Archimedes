#pragma once

namespace archimedes {
	class georectangle;
	class map_projection;
}

namespace archimedes::geoshape {

extern void write_to_image(const QGeoShape &geoshape, QImage &image, const QColor &color, const georectangle &georectangle, const map_projection *map_projection, const std::string &image_checkpoint_save_filename = "");

inline void write_pixel_to_image(const QPoint &pixel_pos, const QColor &color, QImage &image)
{
	if (image.pixelColor(pixel_pos).alpha() != 0) {
		return; //ignore already-written pixels
	}

	image.setPixelColor(pixel_pos, color);
}

}
